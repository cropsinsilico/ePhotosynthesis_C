function out = set_var_from_map(values, options)
    arguments
        values (1,1) dictionary
        options.cond_mod (1,1) string = ""
        options.cond (:,:) double = zeros(0, 0)
        options.ignore_cond (1,1) logical = 0
        options.only_control (1,1) logical = 0
        options.subset (:,:) string = zeros(0, 0)
        options.remove_processed (1,1) logical = 0
        options.subset_preserve (:,:) string = zeros(0, 0)
    end
    out = values;
    if isempty(options.subset)
        options.subset = keys(values);
    end
    % disp("set_var_from_map");
    % options.only_control
    % options.subset
    for i = 1:length(options.subset)
        ikey = options.subset{i};
        if ~isKey(values, ikey)
            continue;
        end
        ires = set_var(ikey, values(ikey), ...
                       cond_mod=options.cond_mod, ...
                       cond=options.cond, ...
                       ignore_cond=options.ignore_cond, ...
                       only_control=options.only_control);
        if options.remove_processed && ires
            if ~any(options.subset_preserve(:) == ikey)
                out = remove(out, ikey);
            end
        end
    end
    global PAR_in_Wpm2;
    global TestLi;
    if PAR_in_Wpm2 == 1
        set_var("TestLi_Wps", TestLi);
        PAR_in_Wpm2 = 0;
    end
end
