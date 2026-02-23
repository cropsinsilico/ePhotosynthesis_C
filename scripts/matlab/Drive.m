function [Tt, d] = Drive(odefun, tspan, y0, options, varargin)
    arguments
        odefun (1,1)
        tspan (1,2) double
        y0 (:,:) double
        options (1,1) struct = struct()
    end
    arguments (Repeating)
        varargin
    end
    global export_mod;
    global import_param_file;
    global import_param_data;
    name = func2str(odefun);
    parts = split(name, "_");
    name = parts{1};
    export_mod = name;
    if ~isempty(import_param_file)
        import_param_data = ReadParam(import_param_file);
        import_param_file = [];
    end
    if ~isempty(import_param_data)
        if numEntries(import_param_data) > 0
            disp("Drive (post-init)");
            import_param_data
            import_param_data = set_var_from_map( ...
                import_param_data, ...
                cond_mod=name, cond=y0, ...
                remove_processed=true, ...
                subset_preserve=get_names("ALL", "COND"))
        end
    end

    global export_mod_first;
    export_mod_first = true;
    
    [Tt,d] = ode15s(odefun, tspan, y0, options, varargin{:});

    global export_mod_level;
    global export_mod_trace;
    if export_mod_level >= 2 || export_mod_trace
        global export_mod_enabled;
        global export_mod_final;
        export_mod_enabled = true;
        export_mod_final = true;
        % Run again to output variables
        odefun(tspan(2), d(size(d, 1), :), varargin{:});
    end
    % Reinitialize some values of global variables.
    % IniModelCom;
end
