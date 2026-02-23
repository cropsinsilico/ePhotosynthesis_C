function values = ePhotosynthesis(driver, model_repo, options)
arguments
    driver (1,1) string
    model_repo (1,1) string
    options.begintime (1,1) double = 0
    options.stoptime (1,1) double = 250
    options.stepsize (1,1) double = 1
    options.abstol (1,1) double = 1e-5
    options.reltol (1,1) double = 1e-4
    options.EnvFile (1,1) string = ""
    options.GRNFile (1,1) string = ""
    options.EnzymeFile (1,1) string = ""
    options.ATPCostFile (1,1) string = ""
    options.OutputFile (1,1) string = "output.data"
    options.OutputParamBase (1,1) string = ""
    options.OutputParamLevel (1,1) double = 0
    options.IterationsFile (1,1) string = ""
    options.WeatherTemp (1,1) double = 25
    options.Inputs (1,1) dictionary = dictionary();
    options.AllowCondition (1,1) logical = false
end

global dontCondition;
global dontConditionCache;
global dontClear;
global dontClearCache;
dontClearCache = dontClear;
dontConditionCache = dontCondition;
if isempty(dontClear) || ~dontClear
    clearvars -except driver model_repo options dontClearCache dontConditionCache;
    global dontClear;
    dontClear = 1; % Prevent clear within drivers
end

global dontCondition;
if options.AllowCondition
    dontCondition = 0;
else
    dontCondition = 1;
end

global global_begintime;
global global_stoptime;
global global_stepsize;
global global_abstol;
global global_reltol;

global_begintime = options.begintime;
global_stoptime = options.stoptime;
global_stepsize = options.stepsize;
global_abstol = options.abstol;
global_reltol = options.reltol;

try

global UseZaksNPQ;
if isempty(UseZaksNPQ)
    UseZaksNPQ = 0;
end

% TODO: Input parameters?
global dontGraph;
dontGraph = 1;

driver = str2double(driver);

% Add model repo to path
addpath(model_repo);

if (strlength(options.OutputParamBase) > 0)
    fprintf("OUTPUT PARAM BASE PROVIDED %s\n", options.OutputParamBase);
    global export_mod_file_base;
    if options.OutputParamLevel == 0
        options.OutputParamLevel = 2;
    end
    export_mod_file_base = options.OutputParamBase;
end
global export_mod_level;
global export_interval;
export_mod_level = options.OutputParamLevel;
export_interval = options.stepsize;  % TODO: Pass step size to ode?

cli_defaults = dictionary( ...
    'ALL::VARS::CO2_in', 400, ...  % TestCa
    'ALL::VARS::TestLi', 470, ...
    'ALL::VARS::ProteinTotalRatio', 0.973, ...
    'ALL::VARS::GRNC', 0, ...
    'ALL::VARS::GRNT', 0, ...
    'BF::MOD::cATPsyn', 1, ...
    'BF::MOD::CPSi', 1, ...
    'BF::MOD::cNADPHsyn', 1, ...
    'FI::MOD::cpsii', 1, ...
    'PR::MOD::RUBISCOTOTAL', 3, ...
    'ALL::VARS::TestATPCost', 0);
if dontCondition
    % These are variables defined in Condition
    cli_defaults('ALL::VARS::Tp') = 25;
    cli_defaults('ALL::VARS::O2') = 0.21 * (3 * 10^4);  % TestO2
    cli_defaults('PR::MOD::RUBISCOMETHOD') = 2;
    cli_defaults('ALL::VARS::VolRatioStCyto') = 1;
end

Env_data = dictionary();
if (strlength(options.EnvFile) > 0)
    fprintf("EVN FILE PROVIDED: %s\n", options.EnvFile);
    Env_data = ReadParam(options.EnvFile);
    Env_data = norm_param_map(Env_data);
end
if (strlength(options.ATPCostFile) > 0)
    fprintf("ATPCOST FILE PROVIDED: %s\n", options.ATPCostFile);
    ATPCost_data = ReadParam(options.ATPCostFile);
    if isKey(ATPCost_data, "ATPCost")
        Env_data('ALL::VARS::TestATPCost') = ATPCost_data("ATPCost");
    end
end
if isConfigured(options.Inputs)
    options.Inputs = norm_param_map(options.Inputs);
    Inputs_keys = keys(options.Inputs);
    for i=1:length(Inputs_keys)
        ikey = Inputs_keys{i};
        Env_data(ikey) = options.Inputs(ikey);
    end
end
% cli_defaults = norm_param_map(cli_defaults);
default_keys = keys(cli_defaults);
for i=1:length(default_keys)
    ikey = default_keys{i};
    if ~isKey(Env_data, ikey)
        Env_data(ikey) = cli_defaults(ikey);
    end
end
Env_data = norm_param_map(Env_data)

if ~isempty(options.IterationsFile)
    global conditionsFile;
    conditionsFile = options.IterationsFile;
end


%%%%%%%%%%%%%%%%%%%%%%%%
% Global parameters that can be read in
%%%%%%%%%%%%%%%%%%%%%%%%


if (strlength(options.GRNFile) > 0)
    set_var_from_map(Env_data, ...
                     subset=["ALL::VARS::GRNC", ...
                             "BF::MOD::cATPsyn", ...
                             "BF::MOD:CPSi", ...
                             "BF::MOD:cNADPHsyn", ...
                             "FI::MOD::cpsii", ...
                             "VfactorC", ...
                             "ALL::VARS::ProteinTotalRatio"]);
    global GRNC;
    global cATPsyn;
    global CPSi;
    global cNADPHsyn;
    global cpsii;
    global VfactorC;
    global pcfactor;
    fprintf("GRN FILE PROVIDED: %s (GRNC = %d)\n", options.GRNFile, GRNC);
    GRN_data = pcfactor .* ReadGRN(options.GRNFile);
    VfactorC = GRN_data(1:33);
    if GRNC == 1
        cATPsyn = GRN_data(34);
        CPSi = GRN_data(35);
        cNADPHsyn = GRN_data(37);
        cpsii = GRN_data(36);
    end
else
    GRN_data = ones(36, 1);
end
if (strlength(options.EnzymeFile) > 0)
    fprintf("ENZYME FILE PROVIDED: %s\n", options.EnzymeFile);
    Enzyme_data = ReadEnzymes(options.EnzymeFile);
    global EnzymeAct;
    EnzymeAct = Enzyme_data(1:27) / 30; % unit change
    global Jmax;
    Jmax = EnzymeAct(27);
    global BFVmax;
    BFVmax = Enzyme_data(28:45);
    global FIVmax;
    FIVmax = Enzyme_data(46:66);
else
    Enzyme_data = ones(66, 1);
end

global import_param_data;
import_param_data = Env_data;

if (driver == 1)
    global TestCa;
    global TestLi;
    global TestATPCost;
    global TestSucPath;
    ResultRate = trDynaPS_Drive(TestCa, TestLi, TestATPCost, TestSucPath, 1, 1);
    Arate = TargetFunVal;
    names = ["Light intensity", "Vc", "Vo", "VPGA", "VT3P", ...
             "Vstarch", "Vt_glycerate", "Vt_glycolate", "CO2AR"];
    nres = size(ResultRate, 1);
    values = zeros(nres+2, 1);
    values(1, 1) = TestLi;
    for i=1:nres
        values(i+1, 1) = ResultRate(i);
    end
    values(nres+2, 1) = Arate;
elseif (driver == 2)
    ResultRate = DynaPS_Drive(1, 1);
    Arate = TargetFunVal;
    names = ["Light intensity", "PSIIabs", "PSIabs", "Vc", "Vo", ...
             "VPGA", "Vstarch", "Vsucrose", "CO2AR"];
    nres = size(ResultRate, 1);
    values = zeros(nres+2, 1);
    global TestLi;
    values(1, 1) = TestLi;
    for i=1:nres
        values(i+1, 1) = ResultRate(i);
    end
    values(nres+2, 1) = Arate;
elseif (driver == 3)
    CM_Drive;
    Arate = TargetFunVal;
    names = ["Light intensity", "CO2AR"];
    global TestLi;
    values = [TestLi; Arate];
elseif (driver == 4)
    % global TestLi;
    % if (strlength(GRNFile) > 0 || strlength(EnzymeFile) > 0)
    %     Arate = EPS_Drive_GRNs(GRN_data, CO2_Env, TestLi, WeatherTemp, ...
    %                            GRNC, GRNT, Enzyme_data);
    % else
    % Arate = EPS_Drive(TestLi, CO2_Env, WeatherTemp);
    % end
    EPS_Drive;
    % global PS_VEL;
    % global PR_VEL;
    % global AVR;
    % [row,col]=size(PS_VEL);
    % % v1 - v1in
    % Arate=(PS_VEL(2,col)-PR_VEL(col,10))*AVR
    Arate = TargetFunVal;
    names = [];
    values = [Arate];
end
WriteTable(options.OutputFile, names, values);

catch ME
    dontClear = dontClearCache;
    dontCondition = dontConditionCache;
    rethrow(ME);
end

end
