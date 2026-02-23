function out = get_names(mod, pt, options)
    arguments
        mod (1,1) string
        pt (1,1) string
        options.full_names (1,1) logical = 0
        options.include_children (1,1) logical = 0
        options.replace_matlab_var (1,1) logical = 0
    end
    if strcmp(pt, "COND_START") == 1
        varname = mod + "_" + pt;
        eval("global " + varname + ";");
        eval("out = " + varname + ";");
    else
        if pt == "MODULES"
            out = [mod];
        else
            eval("out = " + mod + "_NAMES('" + pt + "');");
            if options.replace_matlab_var
                matlab_var = get_names(mod, "MATLAB_VAR");
                if ~isempty(matlab_var)
                    for i = 1:length(out)
                        if isKey(matlab_var, out(i))
                            out(i) = matlab_var(out(i));
                        end
                    end
                end
            end
            if options.full_names
                out = strcat(mod + "::" + pt + "::", out);
            end
        end
        if options.include_children
            out_children = {};
            children = get_names(mod, "CHILDREN");
            for i = 1:length(children)
                out_children{i} = get_names( ...
                    children{i}, pt, ...
                    full_names=options.full_names, ...
                    include_children=options.include_children, ...
                    replace_matlab_var=options.replace_matlab_var);
            end
            % Some composite modules do not use direct concatenation
            % of child conditions so the names need to reflect this
            if mod == "PS_PR" && pt == "COND"
                PSs = out_children{1};
                PRs = out_children{2};
                PS_PRs = strings(0, 0);
                for m=1:4
                    PS_PRs(m) = PSs(m);
                end
                for m = 5:14
                    PS_PRs(m) = PSs(m+1);
                end
                for m = 15:16
                    PS_PRs(m) = PRs(m-14);
                end
                for m = 17:23
                    PS_PRs(m) = PRs(m-13);
                end
                PS_PRs(24) = PSs(5);
                out = cat(2, PS_PRs, out);
            elseif mod == "CM" && pt == "COND"
                CMs = strings(0,0);
                PS_PRs = out_children{1};
                SUCSs = out_children{2};
                for m = 1:23
                    CMs(m) = PS_PRs(m);
                end
                for m=1:12
                    CMs(23+m) = SUCSs(m);
                end
                CMs(36) = PS_PRs(24);
                out = cat(2, CMs, out);
            else
                out = cat(2, out_children{:}, out);
            end
        end
    end
end
