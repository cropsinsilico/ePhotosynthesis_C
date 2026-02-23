function out = export_trace(mod, pt, t, cond)
    arguments
        mod (1,1) string
        pt (1,1) string
        t (1,1) double
        cond (1, :) double = zeros(0, 0)
    end
    global export_mod_trace;
    if ~export_mod_trace
        return;
    end
    global export_mod_enabled;
    trace_var = mod + "_" + pt + "_TRACE";
    N_var = trace_var + "_TIME_N";
    t_var = trace_var + "_OLD_TIME";
    eval("global " + trace_var + ";");
    eval("global " + N_var + ";");
    eval("global " + t_var + ";");
    eval("out = " + trace_var + ";");
    eval("TIME_N = " + N_var + ";");
    eval("OLD_TIME = " + t_var + ";");

    if (isempty(cond))
        % if strcmp(mod, "ALL") == 1
        %     cond = ALL_VALUES(pt);
        % else
        %     cond_var = mod + "_" + pt;
        %     if strcmp(pt, "VEL") == 1
        %         cond_var = mod + "_Vel";
        %     elseif strcmp(pt, "POOL") == 1
        %         cond_var = mod + "_Pool";
        %     end
        %     eval("global " + cond_var + ";");
        %     eval("cond = " + cond_var + ";");
        % end
        % if (isempty(cond))
        %     if isempty(out)
        %         error("Missing " + cond_var);
        %     % else
        %     %     disp("Missing " + cond_var);
        %     end
        %     return;
        % end
        error("Empty trace for " + trace_var);
    end

    if size(cond, 1) == length(cond)
        cond = cond';
    end
    
    if (isempty(TIME_N) || TIME_N == 0 || (export_mod_enabled && t == 0))
        TIME_N = 1;
        OLD_TIME = t;
        out = zeros(1, length(cond) + 1);
    end
    if (t > OLD_TIME)
        TIME_N = TIME_N + 1;
        OLD_TIME = t;
    end

    out(TIME_N, 1) = t;
    for i = 1:length(cond)
        out(TIME_N, i + 1) = cond(i);
    end

    % TODO: Check if this is necessary (i.e. are matlab matrices mutable
    %    objects and modifying a copy through simple assignment will
    %    modify the original).
    eval(N_var + " = TIME_N;");
    eval(t_var + " = OLD_TIME;");
    eval(trace_var + " = out;");
end
