function out = export_mod_data(mod, t0, cond, options)
arguments
    mod (1,1) string
    t0 (1,1) double
    cond (:,:) double
    options.MOD (:,:) double = zeros(0, 0);
    options.VEL (:,:) double = zeros(0, 0);
    options.KE (:,:) double = zeros(0, 0);
    options.RC (:,:) double = zeros(0, 0);
    options.POOL (:,:) double = zeros(0, 0);
    options.create_missing (1,1) logical = false;
end
global export_mod;
global export_mod_out;
global export_mod_enabled;
global export_mod_trace;
global export_mod_level;
global export_mod_first;
global export_mod_final;
global trace_mod_vars;
global export_mod_file_base;
global global_begintime;
global export_interval;
global last_export_time;

if isempty(global_begintime)
    global_begintime = 0;
end
t = t0 + global_begintime;

out = dictionary();

if isempty(export_mod_enabled)
    export_mod_enabled = false;
end
if isempty(export_mod_trace)
    export_mod_trace = false;
end
if isempty(export_mod_level)
    export_mod_level = 0;
end
if isempty(export_mod_first)
    export_mod_first = false;
end
if isempty(export_mod_final)
    export_mod_final = false;
end
if isempty(export_interval)
    export_interval = 1;
end

export_interval_met = false;
if isempty(last_export_time) || export_mod_first || export_mod_final
    export_interval_met = true;
else
    export_interval_met = ((t - last_export_time) >= export_interval);
end

if (export_mod_first || export_mod_final) && isempty(export_mod_file_base)
    export_mod_file_base = export_mod + "_";
end
if export_mod_first
    if export_mod_level >= 1
        export_mod_enabled = true;
    else
        export_mod_enabled = false;
    end
    if export_mod_level >= 3
        disp("Outputing every time step as its own file is really slow, using trace instead.");
        export_mod_level = 2;
        export_mod_trace = true;
    end
elseif export_mod_final
    if export_mod_level >= 2
        export_mod_enabled = true;
    else
        export_mod_enabled = false;
    end
else
    if export_mod_level >= 3 && export_interval_met
        export_mod_enabled = true;
    else
        export_mod_enabled = false;
    end
end

% Defaults set based on other values

if (~export_mod_enabled) && ~(export_mod_trace && export_interval_met)
    export_mod_first = false;
    export_mod_final = false;
    return;
end

extra_param_types = get_names(mod, "PARAM_TYPES");
param_types = {"MOD", "COND", "VEL"};
if ~isempty(extra_param_types)
    extra_param_types = num2cell(extra_param_types);
    param_types = {param_types{:}, extra_param_types{:}};
end
mod_names = {};

children = get_names(mod, "CHILDREN");
if length(children) == 0
    options.COND = cond;
else
    N_cond = length(get_names(mod, "COND"));
    if N_cond == 0
        options.COND = zeros(0, 0);
    else
        start_cond = get_names(mod, "COND_START");
        options.COND = cond(start_cond:(start_cond + N_cond - 1));
    end
end

for i = 1:length(param_types)
    pt = param_types{i};
    if pt == "CALC"
        continue;
    end
    ival = eval("options." + pt);
    if isempty(ival)
        if options.create_missing && (export_mod_trace || export_mod_enabled)
            pt_names = get_names(mod, pt, replace_matlab_var=true);
            cmd = "get_local_var('" + join(pt_names, "', '") + "');";
            ival = evalin('caller', cmd);
            eval("options." + pt + " = ival;");
        else
            continue;
        end
    end
    if size(ival, 1) == length(ival)
        ival = ival';
        eval("options." + pt + " = ival;");
    end
    if ~isempty(ival)
        if export_mod_trace && export_interval_met
            export_trace(mod, pt, t, ival);
        end
        if export_mod_enabled
            names = get_names(mod, pt);
            if length(names) ~= length(ival)
                names
                msg = sprintf("Number of names (%d) dosn't match number of values (%d) for %s::%s", ...
                              length(names), length(ival), mod, pt);
                error(msg);
            elseif size(names) ~= size(ival)
                ival = reshape(ival, size(names));
            end
            out(pt) = dictionary(names, ival);
        end
    end
end

if export_interval_met && strcmp(mod, export_mod) == 1
    last_export_time = t;
end

if export_mod_trace && strcmp(mod, export_mod) == 1
    if export_interval_met
        all_param_types = ALL_NAMES("PARAM_TYPES");
        for i = 1:length(all_param_types)
            pt = all_param_types{i};
            if pt == "CALC"
                continue;
            end
            ival = get_param_type_values("ALL", pt, skip_trace=true);
            if ~isempty(ival)
                export_trace("ALL", pt, t, ival);
            end
        end
    end
    if export_mod_final
        fname = export_mod_file_base + "trace.txt"
        export_trace_dict(fname, trace_mod_vars);
    end
end

if ~export_mod_enabled
    if strcmp(export_mod, mod) == 1
        export_mod_first = false;
        export_mod_final = false;
    end
    return;
end

if isempty(export_mod_out)
    export_mod_out = dictionary();
end

export_mod_out(mod) = out;

if strcmp(export_mod, mod) == 1
    fname = "";
    if export_mod_first
        if export_mod_level >= 1
            fname = export_mod_file_base + "init.txt";
        end
    elseif export_mod_final
        if export_mod_level >= 2
            fname = export_mod_file_base + "last.txt";
        end
    else
        % if export_mod_level >= 3
        %     fname = export_mod_file_base + "step" + string(t) + ".txt";
        % end
    end
    if strlength(fname) > 0
        fprintf("EXPORTING TO %s\n", fname);
        export_mod_out("ALL") = get_module_dict("ALL", include_calc=true);
        dump_vars(fname, export_mod_out);
    end
    export_mod_first = false;
    export_mod_final = false;
    export_mod_out = dictionary();
end

end
