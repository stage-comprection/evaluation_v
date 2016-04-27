#include "mapper.h"

std::string get_BWT(const std::string& s){

    uint n = s.size();
    std::string f = s;
    std::string sa[n];
    sa[0] = s;

    for (uint i=1; i<n; ++i){

        sa[i] = s.substr(n-i, n) + s.substr(0, n-i);
    }

    std::sort(sa, sa+n);

    for (uint i=0; i<n; ++i){

        f[i] = sa[i][n-1];
    }

    return f;
}


std::vector<uint> get_N(const std::string& s){

    std::vector<uint> c {0,0,0,0,0};

    for (uint i=0; i<s.size(); ++i){

        switch(s[i]) {

            case 'A':
                ++c[1];
                break;

            case 'C':
                ++c[2];
                break;

            case 'G':
                ++c[3];
                break;

            case 'T':
                ++c[4];
                break;

            default:
                ++c[0];
                break;
        }
    }

    return c;
}


std::vector<uint> get_R(std::string s){

    uint a=0, c=0, g=0, t=0, d=0;
    std::vector<uint> r;

    for (uint i=0; i<s.size(); i++){

        switch(s[i]) {

            case 'A':
                ++a;
                r.push_back(a);
                break;

            case 'C':
                ++c;
                r.push_back(c);
                break;

            case 'G':
                ++g;
                r.push_back(g);
                break;

            case 'T':
                ++t;
                r.push_back(t);
                break;

            default:
                ++d;
                r.push_back(d);
                break;
        }
    }

    return r;
}



uint LF(char a, uint k, std::vector<uint> N){

    uint l=0;

    switch(a) {

        case 'A':
            l = k;
            break;

        case 'C':
            l = N[1] + k;
            break;

        case 'G':
            l = N[1] + N[2] + k;
            break;

        case 'T':
            l = N[1] + N[2] + N[3] + k;
            break;
    }

    return l;
}



bool search_in_index(std::string query, std::string bwt, std::vector<uint> N, std::vector<uint> R){

    uint start = 0, stop = bwt.size() - 1;
    uint x = start, y = stop;

    for (int i = query.size() - 1; i>=0; --i){

        x = start;
        y = stop;

        while (bwt[x] != query[i] and x <= stop) ++x;
        while (bwt[y] != query[i] and y >= start) --y;

        if (y >= x){

            start = LF(query[i], R[x], N);
            stop = LF(query[i], R[y], N);

        } else {

            return false;
        }
    }

    if ((int) (stop - start) >= 0) return true;
    else return false;
}
