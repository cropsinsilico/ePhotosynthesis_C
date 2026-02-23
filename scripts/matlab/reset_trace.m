function out = reset_trace(mod, pt)
    arguments
        mod (1,1) string
        pt (1,1) string
    end
    global export_mod_trace;
    if isempty(export_mod_trace) || ~export_mod_trace
        return;
    end
    trace_var = mod + "_" + pt + "_TRACE";
    N_var = trace_var + "_TIME_N";
    t_var = trace_var + "_OLD_TIME";
    eval("global " + trace_var + ";");
    eval("global " + N_var + ";");
    eval("global " + t_var + ";");
    eval(t_var + " = 0;");
    eval(N_var + " = 0;");
    eval(trace_var + " = zeros(0, 0);");
    eval("out = " + trace_var + ";");
end
