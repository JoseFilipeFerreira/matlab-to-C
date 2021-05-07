#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define I(N, i, j) ((N) * (i) + (j))

#ifdef DEBUG
#    define DBG(FORMAT, ARGS...) fprintf(stderr, FORMAT, ARGS)
#else
#    define DBG(FORMAT, ARGS...) \
        {}
#endif

void to_image(char* filename, size_t N, float* img) {
    float max = img[0];
    float min = img[0];
    for (size_t i = 1; i < N * N; ++i) {
        max = (max < img[i]) ? img[i] : max;
        min = (min > img[i]) ? img[i] : min;
    }

    FILE* fp = fopen(filename, "w");

    fprintf(fp, "P3\n%zu %zu\n255\n", N, N);

    for (int h = N - 1; h >= 0; --h) {
        for (size_t w = 0; w < N; ++w) {
            float pixel = (img[I(N, h, w)] > 0) ? img[I(N, h, w)] : 0;

            float H = 60 + (240 - 60) * (1 - (pixel + min) / (max + min));

            float X = 1 * (1 - fabs(fmod(H / 60.0, 2) - 1));
            float r, g, b;
            if (H >= 0 && H < 60) {
                r = 1, g = X, b = 0;
            } else if (H >= 60 && H < 120) {
                r = X, g = 1, b = 0;
            } else if (H >= 120 && H < 180) {
                r = 0, g = 1, b = X;
            } else if (H >= 180 && H < 240) {
                r = 0, g = X, b = 1;
            } else if (H >= 240 && H < 300) {
                r = X, g = 0, b = 1;
            } else {
                r = 1, g = 0, b = X;
            }

            fprintf(fp, "%d %d %d\n", (int) (r * 255), (int) (g * 255), (int) (b * 255));
        }
    }

    fclose(fp);
}

void populate_matrix(size_t N, float* w) {
    // 0th line
    for (size_t j = 0; j < N; ++j) {
        w[I(N, 0, j)] = 0;
    }

    // middle lines
    for (size_t i = 1; i < N - 1; ++i) {
        w[I(N, i, 0)] = 100;
        for (size_t j = 1; j < N - 1; ++j) {
            w[I(N, i, j)] = 50;
        }
        w[I(N, i, N - 1)] = 100;
    }

    // last line
    for (size_t j = 0; j < N; ++j) {
        w[I(N, N - 1, j)] = 100;
    }
}

void parallel_populate_matrix(size_t N, float* w) {
    // 0th line
    for (size_t j = 0; j < N; ++j) {
        w[I(N, 0, j)] = 0;
    }

// middle lines
#pragma omp parallel for
    for (size_t i = 1; i < N - 1; ++i) {
        w[I(N, i, 0)] = 100;
        for (size_t j = 1; j < N - 1; ++j) {
            w[I(N, i, j)] = 50;
        }
        w[I(N, i, N - 1)] = 100;
    }

    // last line
    for (size_t j = 0; j < N; ++j) {
        w[I(N, N - 1, j)] = 100;
    }
}

float* poisson_gs(size_t N, float TOL, size_t* ITER) {
    float* w = malloc(sizeof(float) * N * N);
    populate_matrix(N, w);
    float* u = malloc(sizeof(float) * N * N);

    float DIFF = TOL + 1;
    size_t iter = 0;
    for (; DIFF > TOL; ++iter) {
        // store last iteration in u
        memcpy(u, w, sizeof(float) * N * N);

        for (size_t i = 1; i < N - 1; ++i) {
            for (size_t j = 1; j < N - 1; ++j) {
                w[I(N, i, j)] = (w[I(N, i - 1, j)] + w[I(N, i, j - 1)] + w[I(N, i, j + 1)] +
                                 w[I(N, i + 1, j)]) /
                                4;
            }
        }

        DIFF = fabs(w[0] - u[0]);
        for (size_t i = 1; i < N * N; ++i) {
            float n = fabs(w[i] - u[i]);
            DIFF = (DIFF < n) ? n : DIFF;
        }
    }

    free(u);

    *ITER = iter;
    return w;
}

float* poisson_gs_parallel(size_t N, float TOL, size_t* ITER) {
    float* w = malloc(sizeof(float) * N * N);
    populate_matrix(N, w);
    float* u = malloc(sizeof(float) * N * N);

    float DIFF = TOL + 1;
    size_t iter = 0;
    for (; DIFF > TOL; ++iter) {
        // store last iteration in u
        memcpy(u, w, sizeof(float) * N * N);

        for (size_t diag = 1; diag < (N - 1) * 2; ++diag) {
#pragma omp parallel for
            for (size_t i = 1; i <= diag; ++i) {
                int j = diag - i + 1;
                if (j < N - 1 && i < N - 1) {
                    w[I(N, i, j)] = (w[I(N, i - 1, j)] + w[I(N, i, j - 1)] + w[I(N, i, j + 1)] +
                                     w[I(N, i + 1, j)]) /
                                    4;
                }
            }
        }

        DIFF = fabs(w[0] - u[0]);
        for (size_t i = 1; i < N * N; ++i) {
            float n = fabs(w[i] - u[i]);
            DIFF = (DIFF < n) ? n : DIFF;
        }
    }

    free(u);

    *ITER = iter;
    return w;
}

float* poisson_gsrb(size_t N, float TOL, size_t* ITER) {
    float* w = malloc(sizeof(float) * N * N);
    populate_matrix(N, w);
    float* u = malloc(sizeof(float) * N * N);

    float DIFF = TOL + 1;
    size_t iter = 0;
    for (; DIFF > TOL; ++iter) {
        // store last iteration in u
        memcpy(u, w, sizeof(float) * N * N);

        // update red points
        for (size_t i = 1; i < N - 2; ++i) {
            for (size_t j = 1 + (i % 2); j < N - 1; j += 2) {
                w[I(N, i, j)] = (w[I(N, i - 1, j)] + w[I(N, i, j - 1)] + w[I(N, i, j + 1)] +
                                 w[I(N, i + 1, j)]) /
                                4;
            }
        }

        // update black points
        for (size_t i = 1; i < N - 1; ++i) {
            for (size_t j = 1 + ((i + 1) % 2); j < N - 1; j += 2) {
                w[I(N, i, j)] = (w[I(N, i - 1, j)] + w[I(N, i, j - 1)] + w[I(N, i, j + 1)] +
                                 w[I(N, i + 1, j)]) /
                                4;
            }
        }

        DIFF = fabs(w[0] - u[0]);
        for (size_t i = 1; i < N * N; ++i) {
            float n = fabs(w[i] - u[i]);
            DIFF = (DIFF < n) ? n : DIFF;
        }
    }

    free(u);

    *ITER = iter;
    return w;
}

float* poisson_gsrb_parallel(size_t N, float TOL, size_t* ITER) {
    float* w = malloc(sizeof(float) * N * N);
    populate_matrix(N, w);
    float* u = malloc(sizeof(float) * N * N);

    float DIFF = TOL + 1;
    size_t iter = 0;
    for (; DIFF > TOL; ++iter) {
        // store last iteration in u
        memcpy(u, w, sizeof(float) * N * N);

#pragma omp parallel
        {
            // update red points
#pragma omp for
            for (size_t i = 1; i < N - 1; ++i) {
                for (size_t j = 1 + (i % 2); j < N - 1; j += 2) {
                    w[I(N, i, j)] = (w[I(N, i - 1, j)] + w[I(N, i, j - 1)] + w[I(N, i, j + 1)] +
                                     w[I(N, i + 1, j)]) /
                                    4;
                }
            }

            // update black points
#pragma omp for
            for (size_t i = 1; i < N - 1; ++i) {
                for (size_t j = 1 + ((i + 1) % 2); j < N - 1; j += 2) {
                    w[I(N, i, j)] = (w[I(N, i - 1, j)] + w[I(N, i, j - 1)] + w[I(N, i, j + 1)] +
                                     w[I(N, i + 1, j)]) /
                                    4;
                }
            }
        }

        DIFF = fabs(w[0] - u[0]);
        for (size_t i = 1; i < N * N; ++i) {
            float n = fabs(w[i] - u[i]);
            DIFF = (DIFF < n) ? n : DIFF;
        }
    }

    free(u);

    *ITER = iter;
    return w;
}

float* poisson_sorrb(size_t N, float TOL, size_t* ITER) {
    float* w = malloc(sizeof(float) * N * N);
    populate_matrix(N, w);
    float* u = malloc(sizeof(float) * N * N);

    float p = 2.0f / (1.0f + sinf(M_PI / (N - 1)));

    float DIFF = TOL + 1;
    size_t iter = 0;
    for (; DIFF > TOL; ++iter) {
        // store last iteration in u
        memcpy(u, w, sizeof(float) * N * N);

        // update red points
        for (size_t i = 1; i < N - 1; ++i) {
            for (size_t j = 1 + (i % 2); j < N - 1; j += 2) {
                w[I(N, i, j)] =
                    (1 - p) * w[I(N, i, j)] + p *
                                                  (w[I(N, i - 1, j)] + w[I(N, i, j - 1)] +
                                                   w[I(N, i, j + 1)] + w[I(N, i + 1, j)]) /
                                                  4;
            }
        }

        // update black points
        for (size_t i = 1; i < N - 1; ++i) {
            for (size_t j = 1 + ((i + 1) % 2); j < N - 1; j += 2) {
                w[I(N, i, j)] =
                    (1 - p) * w[I(N, i, j)] + p *
                                                  (w[I(N, i - 1, j)] + w[I(N, i, j - 1)] +
                                                   w[I(N, i, j + 1)] + w[I(N, i + 1, j)]) /
                                                  4;
            }
        }

        DIFF = fabs(w[0] - u[0]);
        for (size_t i = 1; i < N * N; ++i) {
            float n = fabs(w[i] - u[i]);
            DIFF = (DIFF < n) ? n : DIFF;
        }
    }

    free(u);

    *ITER = iter;
    return w;
}

float* poisson_sorrb_parallel(size_t N, float TOL, size_t* ITER) {
    float* w = malloc(sizeof(float) * N * N);
    populate_matrix(N, w);
    float* u = malloc(sizeof(float) * N * N);

    float p = 2.0f / (1.0f + sinf(M_PI / (N - 1)));

    float DIFF = TOL + 1;
    size_t iter = 0;
    for (; DIFF > TOL; ++iter) {
        // store last iteration in u
        memcpy(u, w, sizeof(float) * N * N);

#pragma omp parallel
        {
            // update red points
#pragma omp for
            for (size_t i = 1; i < N - 1; ++i) {
                for (size_t j = 1 + (i % 2); j < N - 1; j += 2) {
                    w[I(N, i, j)] =
                        (1 - p) * w[I(N, i, j)] + p *
                                                      (w[I(N, i - 1, j)] + w[I(N, i, j - 1)] +
                                                       w[I(N, i, j + 1)] + w[I(N, i + 1, j)]) /
                                                      4;
                }
            }

            // update black points
#pragma omp for
            for (size_t i = 1; i < N - 1; ++i) {
                for (size_t j = 1 + ((i + 1) % 2); j < N - 1; j += 2) {
                    w[I(N, i, j)] =
                        (1 - p) * w[I(N, i, j)] + p *
                                                      (w[I(N, i - 1, j)] + w[I(N, i, j - 1)] +
                                                       w[I(N, i, j + 1)] + w[I(N, i + 1, j)]) /
                                                      4;
                }
            }
        }

        DIFF = fabs(w[0] - u[0]);
        for (size_t i = 1; i < N * N; ++i) {
            float n = fabs(w[i] - u[i]);
            DIFF = (DIFF < n) ? n : DIFF;
        }
    }

    free(u);

    *ITER = iter;
    return w;
}

void run_test(
    float* f(size_t N, float TOL, size_t* ITER),
    size_t N,
    float TOL,
    size_t* ITER,
    char* filename) {
    clock_t start, end;

    start = clock();
    float* img = f(N, TOL, ITER);
    end = clock();
    to_image(filename, N, img);
    printf("%s  \t%zu iter\t%f sec\n", filename, *ITER, ((double) (end - start) / CLOCKS_PER_SEC));
    free(img);
}

int main(void) {
    size_t N = 200;
    float TOL = 0.02;
    size_t ITER;
    run_test(poisson_gs, N, TOL, &ITER, "poisson_gs.ppm");
    run_test(poisson_gs_parallel, N, TOL, &ITER, "poisson_gs_par.ppm");
    puts("");
    run_test(poisson_gsrb, N, TOL, &ITER, "poisson_gsrb.ppm");
    run_test(poisson_gsrb_parallel, N, TOL, &ITER, "poisson_gsrb_par.ppm");
    puts("");
    run_test(poisson_sorrb, N, TOL, &ITER, "poisson_sorrb.ppm");
    run_test(poisson_sorrb_parallel, N, TOL, &ITER, "poisson_sorrb_par.ppm");
}
