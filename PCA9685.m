classdef PCA9685 < matlab.System ...
        & coder.ExternalDependency ...
        & matlab.system.mixin.Propagates ...
        & matlab.system.mixin.CustomIcon
    %
    % System object template for a source block.
    % 
    % This template includes most, but not all, possible properties,
    % attributes, and methods that you can implement for a System object in
    % Simulink.
    %
    % NOTE: When renaming the class name Source, the file name and
    % constructor name must be updated to use the class name.
    %
    
    % Copyright 2016-2018 The MathWorks, Inc.
    %#codegen
    %#ok<*EMCA>
    
    properties
        SlaveAddress
        Frequency
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.
    end
    
    properties (Access = private)
        % Pre-computed constants.
    end
    
    methods
        % Constructor
        function obj = PCA9685(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end
    end
    
    methods (Access=protected)
        function setupImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation setup code here
            else
                % Call C-function implementing device initialization
                coder.cinclude('rpi_pca9685.h');
                coder.ceval('pca9685IOSetup', uint32(obj.SlaveAddress));
                coder.ceval('pca9685IOSetFrequency', uint32(obj.Frequency));
            end
        end
        
        function stepImpl(obj, u1, u2, u3, u4, u5, u6, u7, u8, u9, u10, u11, u12, u13, u14, u15, u16)   %#ok<MANU>

            if isempty(coder.target)
                % Place simulation output code here
            else
                % Call C-function implementing device output
                coder.ceval('pca9685IOSetPosition', uint32(0), uint32(0), uint32(u1));
                coder.ceval('pca9685IOSetPosition', uint32(1), uint32(0), uint32(u2));
                coder.ceval('pca9685IOSetPosition', uint32(2), uint32(0), uint32(u3));
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('source_terminate');
            end
        end
    end
    
    methods (Access=protected)
        %% Define output properties
        function num = getNumInputsImpl(~)
            num = 12;
        end
        
        function num = getNumOutputsImpl(~)
            num = 0;
        end
        
        function flag = isInputSizeLockedImpl(~,~)
            flag = true;
        end
        
        function varargout = isInputFixedSizeImpl(~,~)
            varargout{1} = true;
        end
        
        function flag = isInputComplexityLockedImpl(~,~)
            flag = true;
        end
        
        function validateInputsImpl(~, u)
            if isempty(coder.target)
                % Run input validation only in Simulation
                validateattributes(u,{'double'},{'scalar'},'','u');
            end
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = 'PCA9685';
        end    
    end
    
    methods (Static, Access=protected)
        function simMode = getSimulateUsingImpl(~)
            simMode = 'Interpreted execution';
        end
        
        function isVisible = showSimulateUsingImpl
            isVisible = false;
        end
    end
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'PCA9685';
        end
        
        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end
        
        function updateBuildInfo(buildInfo, context)
            if context.isCodeGenTarget('rtw')
                % Update buildInfo
                srcDir = fullfile(fileparts(mfilename('fullpath')),'src'); %#ok<NASGU>
%                 libDir = fullfile(fileparts(mfilename('fullpath')),'libraries', 'Adafruit_NeoPixel');
                includeDir = fullfile(fileparts(mfilename('fullpath')),'include');
                
                addIncludePaths(buildInfo,includeDir);
%                 addIncludePaths(buildInfo,libDir);
                % Use the following API's to add include files, sources and
%                 addSourceFiles(buildInfo, 'Adafruit_NeoPixel.cpp', libDir);
                addSourceFiles(buildInfo, 'rpi_pca9685.c', srcDir);
                % linker flags
                addLinkFlags(buildInfo,{'-lwiringPi'});
                addCompileFlags(buildInfo,{'-lwiringPi'});
                %addIncludeFiles(buildInfo,'source.h',includeDir);
                %addSourceFiles(buildInfo,'source.c',srcDir);
                %addLinkFlags(buildInfo,{'-lSource'});
                %addLinkObjects(buildInfo,'sourcelib.a',srcDir);
                %addCompileFlags(buildInfo,{'-D_DEBUG=1'});
                %addDefines(buildInfo,'MY_DEFINE_1')
            end
        end
    end
end
