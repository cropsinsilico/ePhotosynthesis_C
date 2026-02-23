function out = get_var(name, trace, options)
    arguments
        name (1,1) string
        trace (1,1) double = 0
        options.allow_missing (1,1) logical = false
    end
    if trace == 1
        out = get_trace(name, allow_missing=options.allow_missing);
        return;
    end
    parts = split(name, "::");
    mod = parts{1};
    pt = parts{2};
    var = parts{3};
    names = get_names(mod, pt);
    values = get_param_type_values(mod, pt, ...
                                   allow_missing=options.allow_missing);
    if ~any(names(:) == var)
        msg = sprintf("Invalid variable: %s", name);
        error(msg);
    elseif isempty(values)
        msg = sprintf("Missing variable: %s", name);
        if options.allow_missing
            % disp(msg);
            out = 0;
        else
            error(msg);
        end
    elseif length(names) ~= length(values)
        msg = sprintf("Size of names (%d) does not match the size of values (%d) for %s::%s", ...
                     length(names), length(values), mod, pt);
        error(msg);
    else
        idx = find(names == var);
        out = values(idx);
    end
end
