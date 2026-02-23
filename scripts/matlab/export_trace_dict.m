function out = export_trace_dict(fname, vars)
    arguments
        fname (1,1) string = ""
        vars (1,:) string = [];
    end
    out = {};
    % if isempty(vars)
    %     error("EMPTY VARS");
    %     return;
    % end

    names = strings(0,0);
    values = {};
    time = get_trace('PS::COND::time');
    names(1) = 'time';
    values = zeros(length(time), length(vars) + 1);
    values(:, 1) = time;
    
    if isempty(vars)
        global export_mod;
        modules = get_names(export_mod, "MODULES", include_children=true);
        modules = {"ALL", modules{:}};
        for i = 1:length(modules)
            imod = modules{i};
            param_types = get_names(imod, "PARAM_TYPES");
            if (imod ~= "ALL")
                param_types = cat(2, ["MOD", "COND", "VEL"], param_types);
            end
            for j = 1:length(param_types)
                ipt = param_types{j};
                inames = get_names(imod, ipt, full_names=true);
                if isempty(inames)
                    continue;
                end
                trace = import_trace(imod, ipt, include_calc=true);
                names = cat(2, names, inames);
                % imod
                % ipt
                % size(values)
                % size(trace)
                % size(trace(:, 2:size(trace, 2)))
                values = cat(2, values, trace(:, 2:size(trace, 2)));
            end
        end
    else
        for i = 1:length(vars)
            ivar = vars{i};
            names(i + 1) = ivar;
            values(:, i + 1) = get_trace(ivar);
        end
    end

    out = array2table(values, 'VariableNames', names);
    if ~isempty(fname)
        disp("Writing trace data to file: " + fname);
        writetable(out, fname);
    end
end
