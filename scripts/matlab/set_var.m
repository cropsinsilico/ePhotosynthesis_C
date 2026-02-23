function out = set_var(name, value, options)
    arguments
        name (1,1) string
        value (1,1) double
        options.cond_mod (1,1) string = ""
        options.cond (:,:) double = zeros(0, 0)
        options.ignore_cond (1,1) logical = 0
        options.only_control (1,1) logical = 0
        options.verbose (1,1) logical = 0
    end
    out = true;
    parts = split(name, "::");
    if length(parts) == 1
        % parts
        mod = "ALL";
        pt = "VARS";
        var = name;
    else
        mod = parts{1};
        pt = parts{2};
        var = parts{3};
    end
    out = set_param_type_value(mod, pt, var, value, ...
                               cond_mod=options.cond_mod, ...
                               cond=options.cond, ...
                               ignore_cond=options.ignore_cond, ...
                               only_control=options.only_control);
    if out && options.verbose
        fprintf("set_var: %s::%s::%s = %d\n", mod, pt, var, value);
    end
end
