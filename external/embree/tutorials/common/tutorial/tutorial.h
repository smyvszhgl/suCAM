// ======================================================================== //
// Copyright 2009-2017 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "../default.h"
#include "application.h"
#include "camera.h"
#include "scene.h"
#include "scene_device.h"

namespace embree
{
  /* functions provided by each tutorial */
  extern "C" void device_init(const char* cfg);
  extern "C" void device_resize(int width, int height);
  extern "C" void device_render(unsigned* pixels, const unsigned width, const unsigned height, const float time, const ISPCCamera& camera);
  extern "C" bool device_pick(const float x, const float y, const ISPCCamera& camera, Vec3fa& hitPos);
  extern "C" void device_key_pressed (int key);
  extern "C" void device_cleanup();

  template<typename Ty>
    struct Averaged
  {
    Averaged (size_t N, double dt)
    : N(N), dt(dt) {}

    void add(double v)
    {
      values.push_front(std::make_pair(getSeconds(),v));
      if (values.size() > N) values.resize(N);
    }

    Ty get() const
    {
      if (values.size() == 0) return zero;
      double t_begin = values[0].first-dt;

      Ty sum(zero);
      size_t num(0);
      for (size_t i=0; i<values.size(); i++) {
        if (values[i].first >= t_begin) {
          sum += values[i].second;
          num++;
        }
      }
      return sum/Ty(num);
    }

    std::deque<std::pair<double,Ty>> values;
    size_t N;
    double dt;
  };

  class TutorialApplication : public Application
  {
  public:
    TutorialApplication (const std::string& tutorialName, const int features);
    virtual ~TutorialApplication();

  public:
    /* starts tutorial */
    void run(int argc, char** argv);

    /* virtual main function, contains all tutorial logic */
    virtual int main(int argc, char** argv);

    /* callback called after command line parsing finished */
    virtual void postParseCommandLine() {}

    /* benchmark mode */
    void renderBenchmark();

    /* render to file mode */
    void renderToFile(const FileName& fileName);

    /* passes parameters to the backend */
    void set_parameter(size_t parm, ssize_t val);

    /* resize framebuffer */
    void resize(unsigned width, unsigned height);

    /* set scene to use */
    void set_scene (TutorialScene* in);

    /* GLUT callback functions */
  public:
    virtual void keyboardFunc(unsigned char key, int x, int y);
    virtual void keyboardUpFunc(unsigned char key, int x, int y);
    virtual void specialFunc(int key, int, int);
    virtual void clickFunc(int button, int state, int x, int y);
    virtual void motionFunc(int x, int y);
    virtual void displayFunc();
    virtual void reshapeFunc(int width, int height);
    virtual void idleFunc();

  public:
    std::string tutorialName;

    /* render settings */
    Camera camera;
    Shader shader;

    /* framebuffer settings */
    unsigned width;
    unsigned height;
    unsigned* pixels;

    /* image output settings */
    FileName outputImageFilename;

    /* benchmark mode settings */
    size_t skipBenchmarkFrames;
    size_t numBenchmarkFrames;
    size_t numBenchmarkRepetitions;

    /* window settings */
    bool interactive;
    bool fullscreen;

    unsigned window_width;
    unsigned window_height;
    int windowID;

    double time0;
    int debug_int0;
    int debug_int1;

    int mouseMode;
    int clickX, clickY;

    float speed;
    Vec3f moveDelta;

    bool command_line_camera;
    bool print_frame_rate;
    Averaged<double> avg_render_time;
    Averaged<double> avg_frame_time;
    Averaged<double> avg_mrayps;
    bool print_camera;

    int debug0;
    int debug1;
    int debug2;
    int debug3;

    RTCIntersectFlags iflags_coherent;
    RTCIntersectFlags iflags_incoherent;

    std::unique_ptr<ISPCScene> ispc_scene;

  private:
    /* ray statistics */
    void initRayStats();
    int64_t getNumRays();

  public:
    static TutorialApplication* instance;
  };

  class SceneLoadingTutorialApplication : public TutorialApplication
  {
  public:
    SceneLoadingTutorialApplication (const std::string& tutorialName, int features);

    virtual int main(int argc, char** argv);

  public:
    TutorialScene obj_scene;
    Ref<SceneGraph::GroupNode> scene;
    bool convert_tris_to_quads;
    float convert_tris_to_quads_prop;
    bool convert_bezier_to_lines;
    bool convert_hair_to_curves;
    bool convert_bezier_to_bspline;
    bool convert_bspline_to_bezier;
    bool remove_mblur;
    bool remove_non_mblur;
    FileName sceneFilename;
    std::vector<FileName> keyFramesFilenames;
    SceneGraph::InstancingMode instancing_mode;
    std::string subdiv_mode;
    bool print_scene_cameras;
    std::string camera_name;
  };
}
