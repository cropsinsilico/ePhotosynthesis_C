function out = import_trace(mod, pt, options)
    arguments
        mod (1,1) string
        pt (1,1) string
        options.include_calc (1,1) logical = false
        options.allow_missing (1,1) logical = false
    end
    trace_var = mod + "_" + pt + "_TRACE";
    eval("global " + trace_var + ";");
    eval("out = " + trace_var + ";");
    if isempty(out)
        if ~options.allow_missing
            error("Trace not set: " + trace_var);
        end
        return;
    end
    if options.include_calc
        out_calc = get_values(mod, "CALC", ...
                              include_calc=true, trace=true);
        idx_calc = size(out, 2) - size(out_calc, 2) + 1;
        out(:, idx_calc:size(out, 2)) = out_calc(:, :);
    end
end
