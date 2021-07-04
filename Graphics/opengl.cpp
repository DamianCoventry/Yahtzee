#include "stdafx.h"
//#include "opengl.h"
//#include <gl/gl.h>
//#include <gl/glu.h>
//
//namespace
//{
//    struct Color
//    {
//        float r_, g_, b_;
//    };
//
//    Color colors_[8] =
//    {
//        {255.0f, 255.0f,   0.0f},       // 0
//        {255.0f,   0.0f, 255.0f},       // 1
//        {  0.0f, 255.0f, 255.0f},       // 2
//        {255.0f,   0.0f,   0.0f},       // 3
//        {  0.0f, 255.0f,   0.0f},       // 4
//        {  0.0f,   0.0f, 255.0f},       // 5
//        {255.0f, 127.0f,   0.0f},       // 6
//        {  0.0f, 255.0f, 127.0f},       // 7
//    };
//}
//
//OpenGL::OpenGL()
//: dc_(NULL)
//, rc_(NULL)
//{
//}
//
//OpenGL::~OpenGL()
//{
//}
//
//void OpenGL::Start(const Graphics::Window& window)
//{
//    PIXELFORMATDESCRIPTOR format;
//    int index;
//
//    dc_ = GetDC(window.GetHandle());
//    if(NULL == dc_)
//    {
//        throw std::exception("Couldn't get a DC for the window");
//    }
//
//    memset(&format, 0, sizeof(PIXELFORMATDESCRIPTOR));
//
//    format.nSize         = sizeof(PIXELFORMATDESCRIPTOR);
//    format.nVersion      = 1;
//    format.dwFlags       = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
//    format.iPixelType    = PFD_TYPE_RGBA;
//    format.cColorBits    = 32;
//    format.cDepthBits    = 24;
//    format.iLayerType    = PFD_MAIN_PLANE;
//
//    index = ChoosePixelFormat(dc_, &format);
//    if(!index)
//    {
//        throw std::exception("This graphics adapter doesn't support our required pixel format");
//    }
//
//    if(!SetPixelFormat(dc_, index, &format))
//    {
//        throw std::exception("This graphics adapter couldn't set our required pixel format");
//    }
//
//    rc_ = wglCreateContext(dc_);
//    if(NULL == rc_)
//    {
//        throw std::exception("Couldn't create an RC for the window");
//    }
//
//    if(!wglMakeCurrent(dc_, rc_))
//    {
//        throw std::exception("Couldn't make our RC current for the window");
//    }
//
//    int width, height;
//    window.GetSize(&width, &height);
//    glViewport(0, 0, width, height);
//}
//
//void OpenGL::Stop(const Graphics::Window& window)
//{
//    if(rc_ && dc_)
//    {
//        wglMakeCurrent(NULL, NULL);
//        wglDeleteContext(rc_);
//
//        ReleaseDC(window.GetHandle(), dc_);
//    }
//}
//
//void OpenGL::ClearBackBuffer()
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}
//
//void OpenGL::Set3D()
//{
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//
//    gluPerspective(60, 640.0f / 480.0f, 1.0f, 10000.0f);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}
//
//void OpenGL::SetViewpointPos(const Graphics::MyVector& rot, const Graphics::MyVector& pos)
//{
//    glRotatef(-rot.x_, 1.0f, 0.0f, 0.0f);
//    glRotatef(-rot.y_, 0.0f, 1.0f, 0.0f);
//    glTranslatef(-pos.x_, -pos.y_, -pos.z_);
//}
//
////void OpenGL::DrawPolyContainer(const PolyContainer& poly_container)
////{
////    const PolygonList& polygons = poly_container.GetPolygonList();
////    const PointList& points = poly_container.GetPointList();
////
////    glBegin(GL_TRIANGLES);
////    PolygonList::const_iterator polygon;
////    int counter = 0, color_index = 0;
////    for(polygon = polygons.begin(); polygon != polygons.end(); ++polygon)
////    {
////        glColor3f(colors_[color_index].r_, colors_[color_index].g_, colors_[color_index].b_);
////        glVertex3f(points[polygon->p0_index_].x_, points[polygon->p0_index_].y_, points[polygon->p0_index_].z_);
////        glVertex3f(points[polygon->p1_index_].x_, points[polygon->p1_index_].y_, points[polygon->p1_index_].z_);
////        glVertex3f(points[polygon->p2_index_].x_, points[polygon->p2_index_].y_, points[polygon->p2_index_].z_);
////        if(counter++ % 2 == 0)
////        {
////            color_index++;
////            color_index %= 8;
////        }
////    }
////    glEnd();
////}
//
//void OpenGL::DisplayBackBuffer()
//{
//    SwapBuffers(dc_);
//}
