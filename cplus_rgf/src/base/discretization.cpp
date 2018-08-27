//
// Created by mi on 18-8-24.
//

#include "discretization.h"

namespace _discretizationTrainerDense {
    struct Elem {
        float x = 0;
        float y = 0;
        float w = 0;

        bool operator<(const Elem &b) {
            return x < b.x;
        }

        Elem(float _x, float _y, float _w) : x(_x), y(_y), w(_w) {}
    };

    /**
     * 没太明白这个地方使用的gain函数
     */
    class Bucket {
    public:
        size_t begin;
        size_t end;
        size_t cut;
        double lowest_value = 1e-10;
        double max_value = 1e10;
        double gain;


        Bucket(size_t _b, size_t _e, Elem *s_arr,
               double *y_sum_arr, double *w_sum_arr,
               double min_bucket_weights, float lamL2)
                : begin(_b), end(_e), gain(0.0) {
            if (min_bucket_weights < 1e-3) min_bucket_weights = 1e-3;
            if (lamL2 < lowest_value) lamL2 = lowest_value;

            for (size_t my_cut = begin; my_cut < end; my_cut++) {

                if (s_arr[my_cut].x >= s_arr[my_cut + 1].x) {
                    assert(s_arr[my_cut].x == s_arr[my_cut + 1].x);
                    continue;
                }
                double y_sum_left = y_sum_arr[my_cut + 1] - y_sum_arr[begin];
                double weight_sum_left = w_sum_arr[my_cut + 1] - w_sum_arr[begin];

                double y_sum_right = y_sum_arr[end + 1] - y_sum_arr[my_cut + 1];
                double weight_sum_right = w_sum_arr[end + 1] - w_sum_arr[my_cut + 1];

                if (weight_sum_left + lowest_value < min_bucket_weights ||
                    weight_sum_right + lowest_value < min_bucket_weights) {
                    continue;
                }

                double pred_left = y_sum_left / (weight_sum_left + lamL2);
                double pred_right = y_sum_right / (weight_sum_right + lamL2);

                double pred_total = (y_sum_left + y_sum_right) / (weight_sum_left + weight_sum_right + 2 * lamL2);

                double obj_left = (weight_sum_left + lamL2) * pred_left * pred_left - 2 * pred_left * y_sum_left;
                double obj_right = (weight_sum_right + lamL2) * pred_right * pred_right - 2 * pred_right * y_sum_right;

                double obj_tot = (weight_sum_left + weight_sum_right + 2 * lamL2) * pred_total * pred_total -
                                 2 * pred_total * (y_sum_left + y_sum_right);

                double my_gain = obj_tot - (obj_left + obj_right);
                if (my_gain > gain) {
                    cut = my_cut;
                    gain = my_gain;
                }
            }

        }

        float
        train(UniqueArray<float> &boundaries, double min_bucket_weights, unsigned int max_buckets, float lamL2, Elem *s,
              size_t n);
    };

    float Bucket::train(UniqueArray<float> &boundaries, double min_bucket_weights, unsigned int max_buckets,
                        float lamL2, _discretizationTrainerDense::Elem *s, size_t n) {

        if (min_bucket_weights < 1e-3) min_bucket_weights = 1e-3;
        sort(s,s+n);


    }
}