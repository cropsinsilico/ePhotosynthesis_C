function out = norm_param_map(values)
    out = values;
    if isempty(values) || ~isConfigured(values)
        return;
    end
    subset = keys(values);
    for i = 1:length(subset)
        name = subset{i};
        name_alt = norm_param_key(name);
        if name ~= name_alt
            if isKey(out, name_alt)
                error("Parameter already exists: " + name_alt);
            end
            out(name_alt) = out(name);
            out = remove(out, name);
            name = name_alt;
        end
    end
end
