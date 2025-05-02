#include "MonteCarloIntegrator.h"
#include <random>
#include <atomic>
#include <glm/matrix.hpp>
#include <glm/geometric.hpp>
#include "ofLog.h"
#ifdef _OPENMP
#include <omp.h>
#endif

MonteCarloIntegrator::MonteCarloIntegrator(PathTracer& pt, ofCamera* c,
                                           int w, int h, int spp)
    : tracer(pt), cam(c), W(w), H(h), S(std::max(1, spp))
{
    ofLogNotice("MCIntegrator") << "size=" << W << "x" << H << " spp=" << S;
}

void MonteCarloIntegrator::render(ofPixels& out)
{
    out.allocate(W, H, OF_PIXELS_RGB);
    auto* data = out.getData();

    // matrice inverse (Projection * View) pour unProject thread-safe
    glm::mat4 invVP = glm::inverse(cam->getProjectionMatrix() *
                                   cam->getModelViewMatrix());

    std::atomic<int> done{ 0 };
    int step = std::max(1, H / 10);

#pragma omp parallel for schedule(dynamic,1) if(W*H>10000)
    for (int y = 0; y < H; ++y)
    {
        std::default_random_engine eng(
            static_cast<unsigned>(ofGetElapsedTimeMillis()) ^ (y * 9781u));
        std::uniform_real_distribution<float> rng(0.f, 1.f);

        for (int x = 0; x < W; ++x)
        {
            float rAcc = 0, gAcc = 0, bAcc = 0;
            for (int s = 0; s < S; ++s)
            {
                float dx = rng(eng), dy = rng(eng);

                float ndcX = ((x + dx) / float(W)) * 2.f - 1.f;
                float ndcY = 1.f - ((y + dy) / float(H)) * 2.f;

                glm::vec4 np = invVP * glm::vec4(ndcX, ndcY, -1.f, 1.f);
                glm::vec4 fp = invVP * glm::vec4(ndcX, ndcY, 1.f, 1.f);
                np /= np.w; fp /= fp.w;

                auto dir = glm::normalize(glm::vec3(fp) - glm::vec3(np));
                auto col = tracer.trace(Ray(glm::vec3(np), dir), 0);

                rAcc += col.r; gAcc += col.g; bAcc += col.b;
            }
            int idx = (y * W + x) * 3;
            data[idx + 0] = static_cast<unsigned char>(rAcc / S);
            data[idx + 1] = static_cast<unsigned char>(gAcc / S);
            data[idx + 2] = static_cast<unsigned char>(bAcc / S);
        }

        int finished = ++done;
        if (finished % step == 0)
        {
        #pragma omp critical
            ofLogNotice("MCIntegrator") << "progress "
                << finished << "/" << H;
        }
    }
    ofLogNotice("MCIntegrator") << "render finished";
}
