%% Configure

% OS
params.os = computer('arch');
if strfind(params.os, 'win') % Windows
    params.cmd = @(x) dos(x);
else
    params.cmd = @(x) unix(x);
end

% Dataset
params.dataPath = 'data/images';
params.queryPath = 'data/queries';
params.groundtruthPath = 'data/groundtruths';
params.rankListPath = 'data/rankedlistss';
params.tempPath = 'temp';

% Average precision
params.apPath = 'ap';

% Codebook file
params.codebookFile = fullfile(params.dataPath, 'codebook.hdf5');

% All Data BoW file
params.allDataBoWFile = fullfile(params.dataPath, 'all_data_bow.mat');

% All Data Bins file
params.allDataBinsFile = fullfile(params.dataPath, 'all_data_bins.mat');

% All Data Keypoints file
params.allDataKpFile = fullfile(params.dataPath, 'all_data_kp.mat');

% IDF file
params.idfFile = fullfile(params.dataPath, 'idf.mat');

% Arguments
% Default feature extraction arguments
params.featureArgs = '-hesaff -rootsift -noangle';

% Quantize features
params.quantStruct = struct( ...
    'quantize', 'kdtree', ...
    'buildParams', struct('algorithm', 'kdtree', 'trees', 8, 'checks', 800, 'cores', 2), ...
    'knn', 3, ...
    'deltaSqr', 6250, ...
    'topK', 100 ...
);

% Constants
params.kpLen = 5;
params.descLen = 128;
params.histLen = 1000000;

%% Libraries

% VLFeat 0.9.19
run('lib/vlfeat-0.9.19/toolbox/vl_setup.m');

% FLANN 1.8.4
addpath('lib/flann-1.8.4');
