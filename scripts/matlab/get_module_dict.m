function out = get_module_dict(mod, options)
    arguments
        mod (1,1) string
        options.include_calc (1,1) logical = false
    end
    param_types = get_names(mod, "PARAM_TYPES");
    out = dictionary();
    for i = 1:length(param_types)
        pt = param_types{i};
        names = get_names(mod, pt);
        if isempty(names)
            continue;
        end
        values = get_param_type_values( ...
            mod, pt, skip_trace=true, ...
            include_calc=options.include_calc);
        if length(names) ~= length(values)
            names
            values
            msg = sprintf("Number of names (%d) dosn't match number of values (%d) for %s::%s", ...
                          length(names), length(values), mod, pt);
                error(msg);
        elseif size(names) ~= size(values)
            values = reshape(values, size(names));
        end
        out(pt) = dictionary(names, values);
    end
end
