function out = set_param_type_value(mod, pt, var, value, options)
    arguments
        mod (1,1) string
        pt (1,1) string
        var (1,1) string
        value (1,1) double
        options.cond_mod (1,1) string = ""
        options.cond (:,:) double = zeros(0, 0)
        options.ignore_cond (1,1) logical = 0
        options.only_control (1,1) logical = 0
    end
    out = true;

    if options.only_control
        control = get_names(mod, "CTRL");
        if ~any(control(:) == var)
            out = false;
            return;
        end
    end
    aliases = get_names(mod, "ALIASES");
    if ~isempty(aliases) && isKey(aliases, var)
        var = aliases(var);
    end
    if pt == "COND"
        if isempty(options.cond) || isempty(options.cond_mod)
            if options.ignore_cond
                out = false;
                return;
            end
            error("Condition vector not provided");
        end
        names = get_names(options.cond_mod, pt, ...
                          full_names=1, include_children=1);
        if length(names) ~= length(options.cond)
            names
            msg = sprintf("Size of names for %s conditions (%d) does not match the size of the provided conditions vector (%d)", ...
                          options.cond_mod, length(names), ...
                          length(options.cond));
            error(msg);
        end
        name_find = mod + "::" + pt + "::" + var;
    else
        names = get_names(mod, pt);
        name_find = var;
    end
    if ~any(names(:) == name_find)
        names
        error("Invalid name: " + mod + "::" + pt + "::" + var);
    end
    idx = find(names == name_find);
    
    if mod == "ALL" || pt == "MOD"
        matlab_var = get_names(mod, "MATLAB_VAR");
        if ~isempty(matlab_var) && isKey(matlab_var, var)
            var = matlab_var(var);
        end
        set_global_var(var, value);
    elseif pt == "COND"
        options.cond(idx) = value;
    else
        if pt == "RC" || pt == "KE"
            ptvar = mod + "_" + pt;
        else
            ptvar = mod + "_" + ...
                    upper(pt{1,1}(1:1)) + ...
                    lower(pt{1,1}(2:length(pt{1,1})));
        end
        eval("global " + ptvar + ";");
        ivect = eval(ptvar);
        if isempty(ivect)
            % TODO: Test this
            matlab_var = get_names(mod, "MATLAB_VAR");
            if ~isempty(matlab_var) && isKey(matlab_var, var)
                var = matlab_var(var);
            end
            set_global_var(var, value);
        else
            eval(ptvar + "(idx) = value;");
        end
    end
    if mod == "ALL" && pt == "VARS"
        if var == "TestLi"
            global GLight;
            global TestLi;
            global TestLi_Wps;
            GLight = TestLi*0.85*0.85;
            TestLi_Wps = TestLi / (1E6/(2.35E5));
        elseif var == "GLight"
            global GLight;
            global TestLi;
            global TestLi_Wps;
            TestLi = GLight / (0.85*0.85);
            TestLi_Wps = TestLi / (1E6/(2.35E5));
        elseif var == "TestLi_Wps"
            global GLight;
            global TestLi;
            global TestLi_Wps;
            TestLi = TestLi_Wps * (1E6/(2.35E5));
            GLight = TestLi*0.85*0.85;
        elseif var == "CO2_in"
            global TestCa;
            global CO2_cond;
            CO2_cond = (TestCa*0.7) / (3 * 10^4);
        elseif var == "CO2_cond"
            global TestCa;
            global CO2_cond;
            TestCa = (CO2_cond * (3 * 10^4)) / 0.7;
        elseif var == "O2"
            global TestO2;
            global O2_cond;
            O2_cond = TestO2 * 1.26 / (3 * 10^4);
        elseif var == "O2_cond"
            global TestO2;
            global O2_cond;
            TestO2 = O2_cond * (3 * 10^4) / 1.26;
        elseif var == "Tp"
            global Tp;
            global TestTemp;
            global Temp_cond;
            TestTemp = Tp;
            Temp_cond = Tp;
        elseif var == "ProteinTotalRatio"
            global ProteinTotalRatio;
            global pcfactor;
            pcfactor = 1/ProteinTotalRatio;
        elseif var == "pcfactor"
            global ProteinTotalRatio;
            global pcfactor;
            ProteinTotalRatio = 1/pcfactor;
        elseif var == "input_PSI" || var == "input_PSIIcore" || var == "PSIIantennaSize" || var == "PSIantennaSize" || var == "input_LHCII" || var == "input_LHCI"
            global input_PSI;
            global input_PSIIcore;
            global PSIantennaSize;
            global PSIIantennaSize;
            global input_LHCI;
            global input_LHCII;
            global ChlT2;
            global ChlT;
            global ChlPSI;
            ChlT2 = input_PSIIcore * (PSIIantennaSize + 13 * input_LHCII);  % U and A, PSII and LHCII
            ChlT  = PSIIantennaSize * input_PSIIcore; % U , PSII
            ChlPSI = input_PSI * (PSIantennaSize + 13 * input_LHCI);  % U and A of PSI, total Chl in PSI
        % elseif var == "record"
        %     global record;
        %     global export_mod_trace;
        %     if record
        %         export_mod_trace = true;
        %     else
        %         export_mod_trace = false;
        %     end
        end
    end
end
