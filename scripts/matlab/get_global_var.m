function values = get_global_var(varargin)
    names = varargin;
    values = zeros(length(names), 1);
    for i = 1:length(names)
        iname = names{i};
        if iname(1:1) ~= "_"
            try
                eval("global " + iname + ";");
                eval("value = " + iname + ";");
            catch ME
                if ~strcmp(ME.identifier, 'MATLAB:m_illegal_character')
                    rethrow(ME);
                end
            end
            if ~isempty(value)
                values(i) = value;
            end
        end
    end
end
