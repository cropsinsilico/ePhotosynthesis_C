function out = get_values(mod, pt, options)
    arguments
        mod (1,1) string
        pt (1,1) string
        options.include_children (1,1) logical = 0
        options.trace (1,1) logical = 0
        options.include_calc (1,1) logical = 0
    end
    eval("out = " + mod + "_VALUES(pt, trace=options.trace, include_calc=options.include_calc);");
    if options.include_children
        out_children = {};
        children = get_names(mod, "CHILDREN");
        for i = 1:length(children)
            out_children{i} = get_values( ...
                children{i}, pt, ...
                include_children=options.include_children, ...
                trace=options.trace, ...
                include_calc=options.include_calc);
        end
        % Some composite modules do not use direct concatenation
        % of child conditions so the names need to reflect this
        if mod == "PS_PR" && pt == "COND"
            PSs = out_children{1};
            PRs = out_children{2};
            PS_PRs = zeros(0, 0);
            for m=1:4
                PS_PRs(:, m) = PSs(:, m);
            end
            for m = 5:14
                PS_PRs(:, m) = PSs(:, m+1);
            end
            for m = 15:16
                PS_PRs(:, m) = PRs(:, m-14);
            end
            for m = 17:23
                PS_PRs(:, m) = PRs(:, m-13);
            end
            PS_PRs(:, 24) = PSs(:, 5);
            out = cat(2, PS_PRs, out);
        elseif mod == "CM" && pt == "COND"
            CMs = zeros(0, 0);
            PS_PRs = out_children{1};
            SUCSs = out_children{2};
            for m = 1:23
                CMs(:, m) = PS_PRs(:, m);
            end
            for m=1:12
                CMs(:, 23+m) = SUCSs(:, m);
            end
            CMs(:, 36) = PS_PRs(:, 24);
            out = cat(2, CMs, out);
        else
            out = cat(2, out_children{:}, out);
        end
    end
end
