function out = get_param_type_values(mod, pt, options)
    arguments
        mod (1,1) string
        pt (1,1) string
        options.skip_trace (1,1) logical = false
        options.from_local (1,1) logical = false
        options.include_calc (1,1) logical = false
        options.allow_missing (1,1) logical = false
    end
    out = [];

    % From exported dict
    global export_mod_enabled;
    global export_mod_out;
    if export_mod_enabled && ~isempty(export_mod_out) && isKey(export_mod_out, mod)
        xmod = export_mod_out(mod);
        if ~isempty(xmod) && isKey(xmod, pt)
            xpt = xmod(pt);
            names = get_names(mod, pt);
            out = zeros(length(names), 1);
            for i = 1:length(names)
                iname = names(i);
                out(i) = xpt(iname);
            end
        end
    end

    % From variables within the calling context
    if isempty(out) && options.from_local
        names = get_names(mod, pt, replace_matlab_var=true);
        cmd = "get_local_var('" + join(names, "', '") + "');";
        out = evalin('caller', cmd);
    end

    % From exported trace
    global export_mod_trace;
    if isempty(out) && export_mod_trace && ~options.skip_trace
        trace = import_trace(mod, pt, allow_missing=true, ...
                             include_calc=options.include_calc);
        if ~isempty(trace)
            out = trace(size(trace, 1), 2:size(trace, 2));
        end
    end

    % From explicit definition
    if isempty(out)
        out = get_values(mod, pt, include_calc=options.include_calc);
    end

    if isempty(out)
        msg = sprintf("Missing global array or trace for %s::%s", ...
                      mod, pt);
        if options.allow_missing
            disp(msg);
        else
            error(msg);
        end
    end
    
end
