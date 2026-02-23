function out = get_trace(name, options)
    arguments
        name (1,1) string
        options.allow_missing (1,1) logical = false
    end
    parts = split(name, "::");
    mod = parts{1};
    pt = parts{2};
    var = parts{3};
    if strcmp(var, 'time') == 1
        idx = 1;
        names = ['time'];
    else
        names = get_names(mod, pt);
        if ~any(names(:) == var)
            msg = sprintf("Missing variable: %s\n", var);
            error(msg);
        end
        idx = find(names == var);
        idx = idx + 1;
    end
    trace = import_trace(mod, pt, allow_missing=options.allow_missing);
    if isempty(trace) && ~isempty(names) && options.allow_missing
        out = 0;
        return;
    end
    if idx > size(trace, 2)
        name
        idx
        size(trace)
        names
    end
    out = trace(:, idx);
end
