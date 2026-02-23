function fini = Condition_patch_begin(t)

    global UseZaksNPQ;
    global dontCondition;
    if isempty(UseZaksNPQ)
        UseZaksNPQ = 0;
    end
    if isempty(dontCondition)
        dontCondition = 0;
    end

    global conditionsFile;
    global conditionsData;
    global conditionsDataIdx;
    
    if ~isempty(conditionsFile) && strlength(conditionsFile) > 0
        conditionsData = readtable(conditionsFile)
        conditionsFile = [];
        conditionsDataIdx = 0;
    end
    if ~isempty(conditionsData)
        dontCondition = 1;
    end
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%   Read intervals from a table
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    global NumInter_draw;
    
    if ~isempty(conditionsData)
        NumInter_draw = length(conditionsData.time);
        idx = max(find(conditionsData.time <= t));
        if idx ~= conditionsDataIdx
            vars = conditionsData.Properties.VariableNames;
            for i = 1:length(vars)
                ivar = vars{i};
                if strcmp(ivar, "time")
                    continue;
                end
                set_var(ivar, conditionsData{idx, ivar});
            end
            conditionsDataIdx = idx;
        end
    end
    
    if dontCondition
        Condition_patch_end(t);
    end
    
    fini = dontCondition;

end

