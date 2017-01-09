histLen = 1000000;
deltaSqr = 6250;
files = dir('data/termID/*.mat');
files = {files.name};

for i = 1:numel(files)
    binsFile = fullfile('data/termID', files{i});
    weightFile = fullfile('data/weight', files{i});
    bowFile = fullfile('data/bow', files{i});

    
    bins = load(binsFile, '-ascii');
    weights = load(weightFile, '-ascii');

    imageBoW = zeros(histLen, 1);
    imageFreq = zeros(histLen, 1);
    bins = reshape(bins(:), 1, []);

    imageFreq = vl_binsum(imageFreq, ones(size(bins)), bins);
    imageBoW = vl_binsum(imageBoW, weights, bins);

    imageBoW = sqrt(imageBoW) ./ sqrt(imageFreq + 1);

    imageBoW = sparse(imageBoW);
    imageFreq = sparse(imageFreq);
    save(bowFile, 'imageBoW', 'imageFreq');


    fprintf('Finished %d/%d files\n', i, numel(files));
end