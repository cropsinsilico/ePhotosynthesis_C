function Condition_patch_end(t)
    % Variables that might be set via file input
    global TestCa;
    global TestLi;
    global TestO2;
    global TestTemp;

    % Variables set by Condition.m
    global CO2_cond;
    global O2_cond;
    global GLight;
    global Temp_cond;

    global dontCondition;

    global import_param_data;
    if ~isempty(import_param_data)
        if numEntries(import_param_data) > 0
            import_param_data = set_var_from_map( ...
                import_param_data, ...
                remove_processed=dontCondition);
        end
    end
    if ~isempty(import_param_data)
        if numEntries(import_param_data) > 0
            import_param_data
            error("Condition variables were provided via use inputs, but dontCondition was not set");
        end
    end
    
    % Two options here
    if dontCondition
        
        % 1. Override variables set by Condition.m with values set via
        %    file input.
        if isempty(TestO2)
            TestO2 = 0.21;
        end
        if isempty(TestTemp)
            TestTemp = 25;
        end
        
        Temp_cond = TestTemp;
        GLight = TestLi * 0.85 * 0.85;
        CO2Temp = TestCa * 0.7;
        O2Temp = TestO2 / (3 * 10^4);
        CO2_cond = CO2Temp / (3 * 10^4);
        O2_cond = O2Temp * 1.26;

    else
        
        % 2. Set variables expected during output to match values set by
        %    Condition.m
        CO2Temp = CO2_cond * (3 * 10^4);
        O2Temp = O2_cond / 1.26;
        TestCa = CO2Temp / 0.7;
        TestLi = GLight / (0.85 * 0.85);
        TestO2 = O2Temp * (3 * 10^4);
        TestTemp = Temp_cond;
        
    end
    
end
