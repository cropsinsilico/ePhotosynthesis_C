function SYSInitial_patch_end(Begin)
    
%%%%%%%%%%%%%%%%%%%%%%%%
% Set parameters by reading files stored in global variables
%%%%%%%%%%%%%%%%%%%%%%%%

    global tglobal;
    global options1;
    global global_begintime;
    global global_stoptime;
    global global_abstol;
    global global_reltol;

    % Conditions file
    global conditionsFile;
    global conditionsData;
    global conditionsDataIdx;
    if ~isempty(conditionsFile) && strlength(conditionsFile) > 0
        conditionsData = readtable(conditionsFile)
        conditionsFile = [];
    end
    if ~isempty(conditionsData)
        tglobal = max(conditionsData.time);
        conditionsDataIdx = 0;
    elseif ~(isempty(global_stoptime) || isempty(global_begintime))
        tglobal = (global_stoptime - global_begintime);
    end

    if ~isempty(global_reltol)
        options1 = odeset(options1, RelTol=global_reltol);
    end
    if ~isempty(global_abstol)
        options1 = odeset(options1, AbsTol=global_abstol);
    end
    
    % Set variables from dictionary
    global import_param_data;
    if ~isempty(import_param_data)
        if numEntries(import_param_data) > 0
            import_param_data = set_var_from_map( ...
                import_param_data, only_control=true, ...
                remove_processed=true);
            import_param_data = set_var_from_map( ...
                import_param_data, ...
                subset=["ALL::VARS::AVR", ...
                        "ALL::VARS::input_PSI", ...
                        "ALL::VARS::input_PSIIcore", ...
                        "ALL::VARS::PSIantennaSize", ...
                        "ALL::VARS::PSIIantennaSize", ...
                        "ALL::VARS::input_LHCI", ...
                        "ALL::VARS::input_LHCII"], ...
                remove_processed=true);
            disp("SYSInitial (pre-init)");
            import_param_data
        end
    end

    % Initialize variables for tracking
    global registered_modules;
    registered_modules = dictionary();

end
