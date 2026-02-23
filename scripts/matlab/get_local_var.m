function out = get_local_var(varargin)
    names = varargin;
    values = zeros(size(names));
    for i = 1:length(names)
        iname = names{i};
        ival = get_global_var(iname);
        if isempty(ival)
            try
                ival = evalin('caller', iname);
            catch ME
                if (~(strcmp(ME.identifier,'MATLAB:UndefinedFunction') | ...
                      strcmp(ME.identifier, 'MATLAB:m_illegal_character')))
                    rethrow(ME);
                end
            end
        end
        if ~isempty(ival)
            values(i) = ival;
        end
    end
    out = values;
end
