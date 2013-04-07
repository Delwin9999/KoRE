/*
 Copyright (c) 2012 The KoRE Project

  This file is part of KoRE.

  KoRE is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  KoRE is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with KoRE.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <vector>

#include "KoRE/GLerror.h"
#include "KoRE/ShaderProgram.h"
#include "KoRE/Components/MeshComponent.h"
#include "KoRE/Components/TexturesComponent.h"
#include "KoRE/Operations/RenderMesh.h"
#include "KoRE/Operations/BindOperations/BindAttribute.h"
#include "KoRE/Operations/BindOperations/BindUniform.h"
#include "KoRE/Operations/BindOperations/BindTexture.h"
#include "KoRE/Operations/BindOperations/BindImageTexture.h"
#include "KoRE/Operations/OperationFactory.h"
#include "KoRE/Operations/UseFBO.h"
#include "KoRE/Operations/UseShaderProgram.h"
#include "KoRE/Operations/UseAtomicCounterBuffer.h"
#include "KoRE/Operations/ResetAtomicCounterBuffer.h"
#include "KoRE/Operations/MemoryBarrierOp.h"
#include "KoRE/ResourceManager.h"
#include "KoRE/RenderManager.h"
#include "KoRE/Components/Camera.h"
#include "KoRE/SceneNode.h"
#include "KoRE/Timer.h"
#include "KoRE/Texture.h"
#include "KoRE/FrameBuffer.h"
#include "Kore/Passes/FrameBufferStage.h"
#include "Kore/Passes/ShaderProgramPass.h"
#include "KoRE/Passes/NodePass.h"
#include "KoRE/Events.h"
#include "KoRE/TextureSampler.h"

#include "VoxelConeTracing/FullscreenQuad.h"
#include "VoxelConeTracing/Cube.h"
#include "VoxelConeTracing/CubeVolume.h"

#define VOXEL_GRID_RESOLUTION_X 50
#define VOXEL_GRID_RESOLUTION_Y 50
#define VOXEL_GRID_RESOLUTION_Z 50
#define CUBE_SIDELENGTH 1.0f

uint screen_width = 1280;
uint screen_height = 720;
kore::SceneNode* cubeNodes[VOXEL_GRID_RESOLUTION_X][VOXEL_GRID_RESOLUTION_Y][VOXEL_GRID_RESOLUTION_Z];
kore::SceneNode* cameraNode = NULL;
kore::Camera* pCamera = NULL;
kore::Texture* dummyTex3D = NULL;

void initDummyTex3D() {
  using namespace kore;
  dummyTex3D = new kore::Texture;

  STextureProperties texProps;
  texProps.targetType = GL_TEXTURE_3D;
  texProps.width = VOXEL_GRID_RESOLUTION_X;
  texProps.height = VOXEL_GRID_RESOLUTION_Y;
  texProps.depth = VOXEL_GRID_RESOLUTION_Z;
  texProps.format = GL_RGB;
  texProps.internalFormat = GL_RGB8;
  texProps.pixelType = GL_UNSIGNED_BYTE;

  // Create some data
  glm::detail::tvec3<unsigned char> colorValues[VOXEL_GRID_RESOLUTION_X]
                                               [VOXEL_GRID_RESOLUTION_Y]
                                               [VOXEL_GRID_RESOLUTION_Z];

  for (uint z = 0; z < VOXEL_GRID_RESOLUTION_Z; ++z) {
    char valueZ = ((float) z / (float) VOXEL_GRID_RESOLUTION_Z) * 255;
    for (uint y = 0; y < VOXEL_GRID_RESOLUTION_Y; ++y) {
      char valueY = ((float) y / (float) VOXEL_GRID_RESOLUTION_Y) * 255;
      for (uint x = 0; x < VOXEL_GRID_RESOLUTION_X; ++x) {
        char valueX = ((float) x / (float) VOXEL_GRID_RESOLUTION_X) * 255;

        colorValues[x][y][z] = glm::detail::tvec3<unsigned char>(valueX, valueY, valueZ);
      }
    }
  }
  
  dummyTex3D->create(texProps, "Dummy3DTexture", colorValues);
  ResourceManager::getInstance()->addTexture(dummyTex3D);
}

void setupVoxelizeTest() {
  using namespace kore;

  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0f,1.0f,1.0f,1.0f);

  SceneManager* sceneMgr = SceneManager::getInstance();
  ResourceManager* resMgr = ResourceManager::getInstance();
  RenderManager* renderMgr = RenderManager::getInstance();

  // Init Voxelize procedure
  //////////////////////////////////////////////////////////////////////////
  ShaderProgram* voxelizeShader = new ShaderProgram;
  voxelizeShader->
    loadShader("./assets/shader/VoxelConeTracing/voxelizeVert.shader",
    GL_VERTEX_SHADER);

  voxelizeShader->
    loadShader("./assets/shader/VoxelConeTracing/voxelizeGeom.shader",
    GL_GEOMETRY_SHADER);

  voxelizeShader->
    loadShader("./assets/shader/VoxelConeTracing/voxelizeFrag.shader",
    GL_FRAGMENT_SHADER);
  voxelizeShader->init();
  resMgr->addShaderProgram(voxelizeShader);

  ShaderProgramPass* voxelizePass = new ShaderProgramPass;
  voxelizePass->setShaderProgram(voxelizeShader);

  // Init 3D texture sampling procedure 
  //////////////////////////////////////////////////////////////////////////
  // Init Camera
  cameraNode = new kore::SceneNode;
  sceneMgr->getRootNode()->addChild(cameraNode);

  pCamera = new Camera;
  float camOffset = 4.0f;

  cameraNode->translate(glm::vec3(0.5f * (VOXEL_GRID_RESOLUTION_X * CUBE_SIDELENGTH),
                                  0.5f * (VOXEL_GRID_RESOLUTION_Y * CUBE_SIDELENGTH),
                                  VOXEL_GRID_RESOLUTION_Z * CUBE_SIDELENGTH + camOffset),
                                  SPACE_WORLD);

  pCamera->setProjectionPersp(60.0f,
                              (float)screen_width / (float) screen_height,
                              1.0f,
                              500.0f);

  cameraNode->addComponent(pCamera);

  ShaderProgram* cubeSample3DTexShader = new ShaderProgram;
  cubeSample3DTexShader->
    loadShader("./assets/shader/VoxelConeTracing/cubeVolumeVert.shader",
               GL_VERTEX_SHADER);
  cubeSample3DTexShader->
    loadShader("./assets/shader/VoxelConeTracing/cubeVolume_sample3DtexFrag.shader",
               GL_FRAGMENT_SHADER);

  cubeSample3DTexShader->init();
  resMgr->addShaderProgram(cubeSample3DTexShader);

  FrameBufferStage* backBufferStage = new FrameBufferStage;
  GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
  backBufferStage->setFrameBuffer(kore::FrameBuffer::BACKBUFFER,
                                  GL_FRAMEBUFFER, drawBuffers, 1);

  ShaderProgramPass* sample3DtexPass = new ShaderProgramPass;
  sample3DtexPass->setShaderProgram(cubeSample3DTexShader);
    
  //Cube* cubeMesh = new Cube(CUBE_SIDELENGTH);  // init with sidelength 1.0f
  //resMgr->addMesh(cubeMesh);

  CubeVolume* cubeVolumeMesh = new CubeVolume(CUBE_SIDELENGTH,
                                              VOXEL_GRID_RESOLUTION_X,
                                              VOXEL_GRID_RESOLUTION_Y,
                                              VOXEL_GRID_RESOLUTION_Z);

  
  SceneNode* cubeNode = new SceneNode;
  sceneMgr->getRootNode()->addChild(cubeNode);

  MeshComponent* meshComp = new MeshComponent;
  meshComp->setMesh(cubeVolumeMesh);

  cubeNode->addComponent(meshComp);

  initDummyTex3D();
  TexturesComponent* texComp = new TexturesComponent;
  texComp->addTexture(dummyTex3D);
  cubeNode->addComponent(texComp);

  // Setup the correct texture sampler to use
  TexSamplerProperties props;
  props.magfilter = GL_NEAREST;
  props.minfilter = GL_NEAREST;
  props.type = GL_SAMPLER_3D;
  props.wrapping = glm::uvec3(GL_CLAMP, GL_CLAMP, GL_CLAMP);
  cubeSample3DTexShader->setSamplerProperties(0, props);


  // Setup rendering operations
  NodePass* nodePass = new NodePass(cubeNode);
  nodePass->
    addOperation(OperationFactory::create(OP_BINDATTRIBUTE, "v_position",
                                          meshComp, "v_position",
                                          cubeSample3DTexShader));

  nodePass->
    addOperation(OperationFactory::create(OP_BINDUNIFORM, "model Matrix",
                                          cubeNode->getTransform(),
                                          "modelWorld",
                                          cubeSample3DTexShader));

  nodePass->
    addOperation(OperationFactory::create(OP_BINDUNIFORM, "view Matrix",
                                          pCamera, "view",
                                          cubeSample3DTexShader));

  nodePass->
    addOperation(OperationFactory::create(OP_BINDTEXTURE, "Dummy3DTexture",
                                          texComp, "tex3D", cubeSample3DTexShader));

  nodePass->
    addOperation(OperationFactory::create(OP_BINDUNIFORM, "projection Matrix",
                                          pCamera, "proj",
                                          cubeSample3DTexShader));

  nodePass->addOperation(new RenderMesh(meshComp, cubeSample3DTexShader));

  sample3DtexPass->addNodePass(nodePass);
  
  
  backBufferStage->addProgramPass(voxelizePass);
  backBufferStage->addProgramPass(sample3DtexPass);
  renderMgr->addFramebufferStage(backBufferStage);

}

void setupImageLoadStoreTest() {
  using namespace kore;
  glDisable(GL_DEPTH_TEST);  

  glClearColor(1.0f,1.0f,1.0f,1.0f);

  kore::Texture* tex = kore::ResourceManager::getInstance()->
    loadTexture("./assets/textures/Crate.png");

  kore::TexturesComponent* texComponent = new kore::TexturesComponent;
  texComponent->addTexture(tex);
  kore::SceneManager::getInstance()->getRootNode()->addComponent(texComponent);

  FullscreenQuad* fsQuadMesh = FullscreenQuad::getInstance();
  kore::MeshComponent* fsQuadMeshComponent = new kore::MeshComponent;
  fsQuadMeshComponent->setMesh(fsQuadMesh);
  kore::SceneManager::getInstance()->getRootNode()->addComponent(fsQuadMeshComponent);


  kore::ShaderProgram* imgLoadShader = new kore::ShaderProgram;
  imgLoadShader->
    loadShader("./assets/shader/VoxelConeTracing/fullscreenQuad_simple.vert",
                GL_VERTEX_SHADER);
  imgLoadShader->loadShader("./assets/shader/VoxelConeTracing/imageLoad.frag",
                            GL_FRAGMENT_SHADER);
  imgLoadShader->init();

  ShaderProgram* imgStoreShader = new kore::ShaderProgram;
  imgStoreShader->
    loadShader("./assets/shader/VoxelConeTracing/fullscreenQuad_simple.vert",
               GL_VERTEX_SHADER);
  imgStoreShader->
    loadShader("./assets/shader/VoxelConeTracing/imageStore.frag",
               GL_FRAGMENT_SHADER);
  imgStoreShader->init();

  // Setup rendering stages
  kore::FrameBufferStage* backBufferStage = new kore::FrameBufferStage;
  GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
  backBufferStage->setFrameBuffer(kore::FrameBuffer::BACKBUFFER,
                                  GL_FRAMEBUFFER, drawBuffers, 1);
  kore::RenderManager::getInstance()->addFramebufferStage(backBufferStage);

  // imageStore
  ShaderProgramPass* storePass = new ShaderProgramPass(imgStoreShader);
  kore::NodePass* nodePassStore = new NodePass(kore::SceneManager::getInstance()->getRootNode());

  nodePassStore->addOperation(OperationFactory::create(OP_BINDATTRIBUTE,
                                                  "v_position",
                                                  fsQuadMeshComponent,
                                                  "v_position",
                                                  imgStoreShader));

  nodePassStore->addOperation(OperationFactory::create(OP_BINDIMAGETEXTURE,
                                                  tex->getName(),
                                                  texComponent,
                                                  "image", imgStoreShader));

  nodePassStore->addOperation(new RenderMesh(fsQuadMeshComponent, imgStoreShader));
  storePass->addNodePass(nodePassStore);
  backBufferStage->addProgramPass(storePass);


  // imageLoad
  kore::ShaderProgramPass* loadPass = new kore::ShaderProgramPass(imgLoadShader);
  
  NodePass* nodePassLoad = new NodePass(SceneManager::getInstance()->getRootNode());
    
  
  nodePassLoad->addOperation(OperationFactory::create(OP_BINDATTRIBUTE,
                                                  "v_position",
                                                  fsQuadMeshComponent,
                                                  "v_position",
                                                  imgLoadShader));

  nodePassLoad->addOperation(OperationFactory::create(OP_BINDIMAGETEXTURE,
                                                  tex->getName(),
                                                  texComponent,
                                                  "image", imgLoadShader));

  nodePassLoad->addOperation(new RenderMesh(fsQuadMeshComponent, imgLoadShader));

  loadPass->addNodePass(nodePassLoad);
  backBufferStage->addProgramPass(loadPass);
}


void setupAtomicCounterTest() {
  using namespace kore;
  glDisable(GL_DEPTH_TEST);  

  glClearColor(1.0f,1.0f,1.0f,1.0f);

  ShaderProgram* acProg = new ShaderProgram;
  acProg
    ->loadShader("./assets/shader/VoxelConeTracing/fullscreenQuad_simple.vert",
                 GL_VERTEX_SHADER);
  acProg
    ->loadShader("./assets/shader/VoxelConeTracing/atomicCounter.frag",
                  GL_FRAGMENT_SHADER);
  acProg->init();

  FullscreenQuad* fsQuadMesh = FullscreenQuad::getInstance();
  kore::MeshComponent* fsQuadMeshComponent = new kore::MeshComponent;
  fsQuadMeshComponent->setMesh(fsQuadMesh);
  kore::SceneManager::getInstance()->getRootNode()
                                      ->addComponent(fsQuadMeshComponent);

  kore::FrameBufferStage* fboStage = new kore::FrameBufferStage;
  GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
  fboStage->setFrameBuffer(kore::FrameBuffer::BACKBUFFER,
                           GL_FRAMEBUFFER, drawBuffers, 1);
  
  
  ShaderProgramPass* acProgPass = new ShaderProgramPass(acProg);
  
  NodePass* acNodePass =
    new NodePass(SceneManager::getInstance()->getRootNode());
  
  acNodePass->addOperation(OperationFactory::create(OP_BINDATTRIBUTE,
                           "v_position",
                           fsQuadMeshComponent,
                           "v_position",
                           acProg));

  acNodePass->addOperation(
    new UseAtomicCounterBuffer(acProg->getUniform("atomicCounter")));

  acNodePass->addOperation(new RenderMesh(fsQuadMeshComponent, acProg));
  acNodePass->addOperation(new ResetAtomicCounterBuffer(acProg->getUniform("atomicCounter"), 0));
  acNodePass->addOperation(new MemoryBarrierOp(GL_ATOMIC_COUNTER_BARRIER_BIT));

  acProgPass->addNodePass(acNodePass);
  fboStage->addProgramPass(acProgPass);
  RenderManager::getInstance()->addFramebufferStage(fboStage);
}

int main(void) {
  int running = GL_TRUE;

  // Initialize GLFW
  if (!glfwInit()) {
    kore::Log::getInstance()->write("[ERROR] could not load window manager\n");
    exit(EXIT_FAILURE);
  }

  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  // Open an OpenGL window
  if (!glfwOpenWindow(screen_width, screen_height, 8, 8, 8, 8, 24, 8, GLFW_WINDOW)) {
    kore::Log::getInstance()->write("[ERROR] could not open render window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // disable v-sync
  glfwSwapInterval(0);

  // initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit()) {
    kore::Log::getInstance()->write("[ERROR] could not open initialize extension manager\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  // log versions
  int GLFWmajor, GLFWminor, GLFWrev;
  glfwGetVersion(&GLFWmajor, &GLFWminor, &GLFWrev);
  kore::Log::getInstance()
    ->write("Render Device: %s\n",
            reinterpret_cast<const char*>(
            glGetString(GL_RENDERER)));
  kore::Log::getInstance()
    ->write("Vendor: %s\n",
            reinterpret_cast<const char*>(
            glGetString(GL_VENDOR)));
  kore::Log::getInstance()
    ->write("OpenGL version: %s\n",
            reinterpret_cast<const char*>(
            glGetString(GL_VERSION)));
  kore::Log::getInstance()
    ->write("GLSL version: %s\n",
             reinterpret_cast<const char*>(
             glGetString(GL_SHADING_LANGUAGE_VERSION)));
  kore::Log::getInstance()
    ->write("GLFW version %i.%i.%i\n",
             GLFWmajor, GLFWminor, GLFWrev);
  kore::Log::getInstance()
    ->write("GLEW version: %s\n",
            reinterpret_cast<const char*>(
            glewGetString(GLEW_VERSION)));

//  setupImageLoadStoreTest();
  // setupAtomicCounterTest();
  setupVoxelizeTest();

  kore::Timer the_timer;
  the_timer.start();
  double time = 0;
  float cameraMoveSpeed = 4.0f;
  
  int oldMouseX = 0;
  int oldMouseY = 0;
  glfwGetMousePos(&oldMouseX,&oldMouseY);
    
  // Main loop
  while (running) {
    time = the_timer.timeSinceLastCall();
    kore::SceneManager::getInstance()->update();

    if (pCamera) {
      if (glfwGetKey(GLFW_KEY_UP) || glfwGetKey('W')) {
      
        pCamera->moveForward(cameraMoveSpeed * static_cast<float>(time));
      }

      if (glfwGetKey(GLFW_KEY_DOWN) || glfwGetKey('S')) {
        pCamera->moveForward(-cameraMoveSpeed * static_cast<float>(time));
      }

      if (glfwGetKey(GLFW_KEY_LEFT) || glfwGetKey('A')) {
        pCamera->moveSideways(-cameraMoveSpeed * static_cast<float>(time));
      }

      if (glfwGetKey(GLFW_KEY_RIGHT) || glfwGetKey('D')) {
        pCamera->moveSideways(cameraMoveSpeed * static_cast<float>(time));
      }

      int mouseX = 0;
      int mouseY = 0;
      glfwGetMousePos(&mouseX,&mouseY);

      int mouseMoveX = mouseX - oldMouseX;
      int mouseMoveY = mouseY - oldMouseY;

      if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_1) == GLFW_PRESS ) {
        if (glm::abs(mouseMoveX) > 0 || glm::abs(mouseMoveY) > 0) {
          pCamera->rotateFromMouseMove((float)-mouseMoveX / 5.0f,
            (float)-mouseMoveY / 5.0f);
        }
      }

      oldMouseX = mouseX;
      oldMouseY = mouseY;
    }

    kore::GLerror::gl_ErrorCheckStart();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);
    kore::RenderManager::getInstance()->renderFrame();

    glfwSwapBuffers();
    kore::GLerror::gl_ErrorCheckFinish("Main Loop");

    // Check if ESC key was pressed or window was closed
    running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
  }

  // Close window and terminate GLFW
  glfwTerminate();

  // Exit program
  exit(EXIT_SUCCESS);
};
