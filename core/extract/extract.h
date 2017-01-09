#ifndef EXTRACT_H_INCLUDED
#define EXTRACT_H_INCLUDED

#include "../configurations.h"
#include "../utils/hesaff/hesaff.h"

using namespace arma;

arma::mat inv2x2(arma::mat C) {
    arma::mat den = C.row(0) % C.row(2) - C.row(1) % C.row(1);
    arma::mat S = join_vert(join_vert(C.row(2), - C.row(1)), C.row(0)) / repmat(den.row(0), 3, 1);
    return S;
}

bool vl_ubcread(const string &file, arma::mat &f, arma::umat &d) {
    int numKeypoints, descLen;
    ifstream fid(file.c_str());
    if (!fid.good())
        return false;

    fid >> descLen >> numKeypoints;

    f = arma::mat(5, numKeypoints);

    d = arma::umat(descLen, numKeypoints);

    for (int k = 0; k < numKeypoints; ++k) {
        fid >> f(0, k) >> f(1, k) >> f(2, k) >> f(3, k) >> f(4, k);
        for (int i = 0; i < descLen; ++i)
            fid >> d(i, k);
    }
    fid.close();

    f.rows(0, 1) = f.rows(0, 1) + 1;
    f.rows(2, 4) = inv2x2(f.rows(2, 4));

    return 1;
}

void extractFeatures(string imagePath, arma::mat &kpMat, arma::mat &siftMat, const string &kpPath, const string &siftPath, const string &tempPath, bool force = false) {
    if (!force && file_exists(siftPath)) {
        kpMat.load(kpPath);
        siftMat.load(siftPath);
        return;
    }

    string tempFile = "./temp.mat";

    hesaff(imagePath, tempFile);

    // string cmd = computeDescriptorPath + " " + imagePath + " " + tempFile;

    // system(cmd.c_str());

    arma::mat clip_kp;
    arma::umat clip_desc;

    if (!vl_ubcread(tempFile, clip_kp, clip_desc)) {
        clip_kp = arma::mat(5, 0);
        clip_desc = arma::umat(128, 0);
    }

    // debugVar(clip_desc(2, 0));

    arma::mat sift = arma::conv_to<arma::mat>::from(clip_desc);

    arma::mat sqrt_desc = sqrt(sift / repmat(sum(clip_desc), 128, 1));

    // debugVar(sqrt_desc(2, 0));

    kpMat = clip_kp;
    siftMat = sqrt_desc;
    kpMat.save(kpPath);
    siftMat.save(siftPath);
}

#endif
