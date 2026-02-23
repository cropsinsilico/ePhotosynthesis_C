function out = norm_param_key(name)
    out = name;
    parts = split(name, "::");
    if length(parts) == 3
        mod = parts{1};
        pt = parts{2};
        names = get_names(mod, pt);
        if ~any(names(:) == parts{3})
            aliases = get_names(mod, "ALIASES");
            if ~isempty(aliases) && isKey(aliases, parts{3})
                out = mod + "::" + pt + "::" + aliases(parts{3});
            end
        end
        return;
    end
    if length(parts) ~= 1
        error("Failed to parse name: " + name);
    end
    modules = ALL_NAMES("MODULES");
    modules = ['ALL', modules];
    for i = 1:length(modules)
        mod = modules{i};
        if mod == "ALL"
            param_types = {};
        else
            param_types = {"MOD", "COND", "VEL"};
        end
        extra_param_types = get_names(mod, "PARAM_TYPES");
        if ~isempty(extra_param_types)
            extra_param_types = num2cell(extra_param_types);
            param_types = {param_types{:}, extra_param_types{:}};
        end
        aliases = get_names(mod, "ALIASES");
        if ~isempty(aliases) && isKey(aliases, name)
            name = aliases(name);
        end
        for j = 1:length(param_types)
            pt = param_types{j};
            names = get_names(mod, pt);
            if any(names(:) == name)
                out = mod + "::" + pt + "::" + name;
                return;
            end
        end
    end
end
