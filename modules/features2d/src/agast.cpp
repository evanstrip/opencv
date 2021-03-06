/* This is AGAST and OAST, an optimal and accelerated corner detector
              based on the accelerated segment tests
   Below is the original copyright and the references */

/*
Copyright (C) 2010  Elmar Mair
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    *Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

    *Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

    *Neither the name of the University of Cambridge nor the names of
     its contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
The references are:
 * Adaptive and Generic Corner Detection Based on the Accelerated Segment Test,
   Elmar Mair and Gregory D. Hager and Darius Burschka
   and Michael Suppa and Gerhard Hirzinger ECCV 2010
   URL: http://www6.in.tum.de/Main/ResearchAgast
*/

#include "precomp.hpp"
#include "agast_score.hpp"

#ifdef _WIN32
#pragma warning( disable : 4127 )
#endif

namespace cv
{

static void AGAST_5_8(InputArray _img, std::vector<KeyPoint>& keypoints, int threshold)
{

    cv::Mat img;
    if(!_img.getMat().isContinuous())
      img = _img.getMat().clone();
    else
      img = _img.getMat();

    size_t total = 0;
    int xsize = img.cols;
    int ysize = img.rows;
    size_t nExpectedCorners = keypoints.capacity();
    register int x, y;
    register int xsizeB = xsize - 2;
    register int ysizeB = ysize - 1;
    register int width;

    keypoints.resize(0);

    int pixel_5_8_[16];
    makeAgastOffsets(pixel_5_8_, (int)img.step, AgastFeatureDetector::AGAST_5_8);

    register short offset0 = (short) pixel_5_8_[0];
    register short offset1 = (short) pixel_5_8_[1];
    register short offset2 = (short) pixel_5_8_[2];
    register short offset3 = (short) pixel_5_8_[3];
    register short offset4 = (short) pixel_5_8_[4];
    register short offset5 = (short) pixel_5_8_[5];
    register short offset6 = (short) pixel_5_8_[6];
    register short offset7 = (short) pixel_5_8_[7];

    width = xsize;

    for(y = 1; y < ysizeB; y++)
    {
        x = 0;
        while(true)
        {
          homogeneous:
          {
            x++;
            if(x > xsizeB)
                break;
            else
            {
                register const unsigned char* const ptr = img.ptr() + y*width + x;
                register const int cb = *ptr + threshold;
                register const int c_b = *ptr - threshold;
                if(ptr[offset0] > cb)
                  if(ptr[offset2] > cb)
                    if(ptr[offset3] > cb)
                      if(ptr[offset5] > cb)
                        if(ptr[offset1] > cb)
                          if(ptr[offset4] > cb)
                            goto success_structured;
                          else
                            if(ptr[offset7] > cb)
                              goto success_structured;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset4] > cb)
                            if(ptr[offset6] > cb)
                              goto success_structured;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset1] > cb)
                          if(ptr[offset4] > cb)
                            goto success_homogeneous;
                          else
                            if(ptr[offset7] > cb)
                              goto success_homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                    else
                      if(ptr[offset7] > cb)
                        if(ptr[offset6] > cb)
                          if(ptr[offset5] > cb)
                            if(ptr[offset1] > cb)
                              goto success_structured;
                            else
                              if(ptr[offset4] > cb)
                                goto success_structured;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset1] > cb)
                              goto success_homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        if(ptr[offset5] < c_b)
                          if(ptr[offset3] < c_b)
                            if(ptr[offset7] < c_b)
                              if(ptr[offset4] < c_b)
                                if(ptr[offset6] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                  else
                    if(ptr[offset5] > cb)
                      if(ptr[offset7] > cb)
                        if(ptr[offset6] > cb)
                          if(ptr[offset1] > cb)
                            goto success_homogeneous;
                          else
                            if(ptr[offset4] > cb)
                              goto success_homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        goto homogeneous;
                    else
                      if(ptr[offset5] < c_b)
                        if(ptr[offset3] < c_b)
                          if(ptr[offset2] < c_b)
                            if(ptr[offset1] < c_b)
                              if(ptr[offset4] < c_b)
                                goto success_structured;
                              else
                                goto homogeneous;
                            else
                              if(ptr[offset4] < c_b)
                                if(ptr[offset6] < c_b)
                                  goto success_structured;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset7] < c_b)
                              if(ptr[offset4] < c_b)
                                if(ptr[offset6] < c_b)
                                  goto success_structured;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        goto homogeneous;
                else if(ptr[offset0] < c_b)
                  if(ptr[offset2] < c_b)
                    if(ptr[offset7] > cb)
                      if(ptr[offset3] < c_b)
                        if(ptr[offset5] < c_b)
                          if(ptr[offset1] < c_b)
                            if(ptr[offset4] < c_b)
                              goto success_structured;
                            else
                              goto structured;
                          else
                            if(ptr[offset4] < c_b)
                              if(ptr[offset6] < c_b)
                                goto success_structured;
                              else
                                goto structured;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset1] < c_b)
                            if(ptr[offset4] < c_b)
                              goto success_structured;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset5] > cb)
                          if(ptr[offset3] > cb)
                            if(ptr[offset4] > cb)
                              if(ptr[offset6] > cb)
                                goto success_structured;
                              else
                                goto structured;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                    else
                      if(ptr[offset7] < c_b)
                        if(ptr[offset3] < c_b)
                          if(ptr[offset5] < c_b)
                            if(ptr[offset1] < c_b)
                              goto success_structured;
                            else
                              if(ptr[offset4] < c_b)
                                if(ptr[offset6] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset1] < c_b)
                              goto success_homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset6] < c_b)
                            if(ptr[offset5] < c_b)
                              if(ptr[offset1] < c_b)
                                goto success_structured;
                              else
                                if(ptr[offset4] < c_b)
                                  goto success_structured;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset1] < c_b)
                                goto success_homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset3] < c_b)
                          if(ptr[offset5] < c_b)
                            if(ptr[offset1] < c_b)
                              if(ptr[offset4] < c_b)
                                goto success_structured;
                              else
                                goto homogeneous;
                            else
                              if(ptr[offset4] < c_b)
                                if(ptr[offset6] < c_b)
                                  goto success_structured;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset1] < c_b)
                              if(ptr[offset4] < c_b)
                                goto success_homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                  else
                    if(ptr[offset5] > cb)
                      if(ptr[offset3] > cb)
                        if(ptr[offset2] > cb)
                          if(ptr[offset1] > cb)
                            if(ptr[offset4] > cb)
                              goto success_structured;
                            else
                              goto homogeneous;
                          else
                            if(ptr[offset4] > cb)
                              if(ptr[offset6] > cb)
                                goto success_structured;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset7] > cb)
                            if(ptr[offset4] > cb)
                              if(ptr[offset6] > cb)
                                goto success_structured;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        goto homogeneous;
                    else
                      if(ptr[offset5] < c_b)
                        if(ptr[offset7] < c_b)
                          if(ptr[offset6] < c_b)
                            if(ptr[offset1] < c_b)
                              goto success_homogeneous;
                            else
                              if(ptr[offset4] < c_b)
                                goto success_homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        goto homogeneous;
                else
                  if(ptr[offset3] > cb)
                    if(ptr[offset5] > cb)
                      if(ptr[offset2] > cb)
                        if(ptr[offset1] > cb)
                          if(ptr[offset4] > cb)
                            goto success_homogeneous;
                          else
                            goto homogeneous;
                        else
                          if(ptr[offset4] > cb)
                            if(ptr[offset6] > cb)
                              goto success_homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset7] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset6] > cb)
                              goto success_homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                    else
                      goto homogeneous;
                  else
                    if(ptr[offset3] < c_b)
                      if(ptr[offset5] < c_b)
                        if(ptr[offset2] < c_b)
                          if(ptr[offset1] < c_b)
                            if(ptr[offset4] < c_b)
                              goto success_homogeneous;
                            else
                              goto homogeneous;
                          else
                            if(ptr[offset4] < c_b)
                              if(ptr[offset6] < c_b)
                                goto success_homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset7] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset6] < c_b)
                                goto success_homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        goto homogeneous;
                    else
                      goto homogeneous;
            }
          }
          structured:
          {
            x++;
            if(x > xsizeB)
                break;
            else
            {
                register const unsigned char* const ptr = img.ptr() + y*width + x;
                register const int cb = *ptr + threshold;
                register const int c_b = *ptr - threshold;
                if(ptr[offset0] > cb)
                  if(ptr[offset2] > cb)
                    if(ptr[offset3] > cb)
                      if(ptr[offset5] > cb)
                        if(ptr[offset7] > cb)
                          if(ptr[offset1] > cb)
                            goto success_structured;
                          else
                            if(ptr[offset4] > cb)
                              if(ptr[offset6] > cb)
                                goto success_structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset1] > cb)
                            if(ptr[offset4] > cb)
                              goto success_structured;
                            else
                              goto structured;
                          else
                            if(ptr[offset4] > cb)
                              if(ptr[offset6] > cb)
                                goto success_structured;
                              else
                                goto structured;
                            else
                              goto structured;
                      else
                        if(ptr[offset7] > cb)
                          if(ptr[offset1] > cb)
                            goto success_structured;
                          else
                            goto structured;
                        else
                          if(ptr[offset1] > cb)
                            if(ptr[offset4] > cb)
                              goto success_structured;
                            else
                              goto structured;
                          else
                            goto structured;
                    else
                      if(ptr[offset7] > cb)
                        if(ptr[offset6] > cb)
                          if(ptr[offset5] > cb)
                            if(ptr[offset1] > cb)
                              goto success_structured;
                            else
                              if(ptr[offset4] > cb)
                                goto success_structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset1] > cb)
                              goto success_structured;
                            else
                              goto structured;
                        else
                          goto structured;
                      else
                        if(ptr[offset5] < c_b)
                          if(ptr[offset3] < c_b)
                            if(ptr[offset7] < c_b)
                              if(ptr[offset4] < c_b)
                                if(ptr[offset6] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto structured;
                  else
                    if(ptr[offset5] > cb)
                      if(ptr[offset7] > cb)
                        if(ptr[offset6] > cb)
                          if(ptr[offset1] > cb)
                            goto success_structured;
                          else
                            if(ptr[offset4] > cb)
                              goto success_structured;
                            else
                              goto structured;
                        else
                          goto structured;
                      else
                        goto structured;
                    else
                      if(ptr[offset5] < c_b)
                        if(ptr[offset3] < c_b)
                          if(ptr[offset2] < c_b)
                            if(ptr[offset1] < c_b)
                              if(ptr[offset4] < c_b)
                                goto success_structured;
                              else
                                goto structured;
                            else
                              if(ptr[offset4] < c_b)
                                if(ptr[offset6] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset7] < c_b)
                              if(ptr[offset4] < c_b)
                                if(ptr[offset6] < c_b)
                                  goto success_homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto structured;
                      else
                        goto homogeneous;
                else if(ptr[offset0] < c_b)
                  if(ptr[offset2] < c_b)
                    if(ptr[offset7] > cb)
                      if(ptr[offset3] < c_b)
                        if(ptr[offset5] < c_b)
                          if(ptr[offset1] < c_b)
                            if(ptr[offset4] < c_b)
                              goto success_structured;
                            else
                              goto structured;
                          else
                            if(ptr[offset4] < c_b)
                              if(ptr[offset6] < c_b)
                                goto success_structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset1] < c_b)
                            if(ptr[offset4] < c_b)
                              goto success_structured;
                            else
                              goto structured;
                          else
                            goto structured;
                      else
                        if(ptr[offset5] > cb)
                          if(ptr[offset3] > cb)
                            if(ptr[offset4] > cb)
                              if(ptr[offset6] > cb)
                                goto success_structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto homogeneous;
                        else
                          goto structured;
                    else
                      if(ptr[offset7] < c_b)
                        if(ptr[offset3] < c_b)
                          if(ptr[offset5] < c_b)
                            if(ptr[offset1] < c_b)
                              goto success_structured;
                            else
                              if(ptr[offset4] < c_b)
                                if(ptr[offset6] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset1] < c_b)
                              goto success_structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset6] < c_b)
                            if(ptr[offset5] < c_b)
                              if(ptr[offset1] < c_b)
                                goto success_structured;
                              else
                                if(ptr[offset4] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset1] < c_b)
                                goto success_structured;
                              else
                                goto structured;
                          else
                            goto structured;
                      else
                        if(ptr[offset3] < c_b)
                          if(ptr[offset5] < c_b)
                            if(ptr[offset1] < c_b)
                              if(ptr[offset4] < c_b)
                                goto success_homogeneous;
                              else
                                goto homogeneous;
                            else
                              if(ptr[offset4] < c_b)
                                if(ptr[offset6] < c_b)
                                  goto success_homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset1] < c_b)
                              if(ptr[offset4] < c_b)
                                goto success_homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                  else
                    if(ptr[offset5] > cb)
                      if(ptr[offset3] > cb)
                        if(ptr[offset2] > cb)
                          if(ptr[offset1] > cb)
                            if(ptr[offset4] > cb)
                              goto success_structured;
                            else
                              goto structured;
                          else
                            if(ptr[offset4] > cb)
                              if(ptr[offset6] > cb)
                                goto success_structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset7] > cb)
                            if(ptr[offset4] > cb)
                              if(ptr[offset6] > cb)
                                goto success_homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        goto structured;
                    else
                      if(ptr[offset5] < c_b)
                        if(ptr[offset7] < c_b)
                          if(ptr[offset6] < c_b)
                            if(ptr[offset1] < c_b)
                              goto success_structured;
                            else
                              if(ptr[offset4] < c_b)
                                goto success_structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                      else
                        goto homogeneous;
                else
                  if(ptr[offset3] > cb)
                    if(ptr[offset5] > cb)
                      if(ptr[offset2] > cb)
                        if(ptr[offset1] > cb)
                          if(ptr[offset4] > cb)
                            goto success_homogeneous;
                          else
                            goto homogeneous;
                        else
                          if(ptr[offset4] > cb)
                            if(ptr[offset6] > cb)
                              goto success_homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset7] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset6] > cb)
                              goto success_homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                    else
                      goto homogeneous;
                  else
                    if(ptr[offset3] < c_b)
                      if(ptr[offset5] < c_b)
                        if(ptr[offset2] < c_b)
                          if(ptr[offset1] < c_b)
                            if(ptr[offset4] < c_b)
                              goto success_homogeneous;
                            else
                              goto homogeneous;
                          else
                            if(ptr[offset4] < c_b)
                              if(ptr[offset6] < c_b)
                                goto success_homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset7] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset6] < c_b)
                                goto success_homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        goto homogeneous;
                    else
                      goto homogeneous;
            }
          }
            success_homogeneous:
            if(total == nExpectedCorners)
            {
                if(nExpectedCorners == 0)
                {
                    nExpectedCorners = 512;
                    keypoints.reserve(nExpectedCorners);
                }
                else
                {
                    nExpectedCorners *= 2;
                    keypoints.reserve(nExpectedCorners);
                }
            }
            keypoints.push_back(KeyPoint(Point2f((float)x, (float)y), 1.0f));
            total++;
            goto homogeneous;
            success_structured:
            if(total == nExpectedCorners)
            {
                if(nExpectedCorners == 0)
                {
                    nExpectedCorners = 512;
                    keypoints.reserve(nExpectedCorners);
                }
                else
                {
                    nExpectedCorners *= 2;
                    keypoints.reserve(nExpectedCorners);
                }
            }
            keypoints.push_back(KeyPoint(Point2f((float)x, (float)y), 1.0f));
            total++;
            goto structured;
        }
    }
}

static void AGAST_7_12d(InputArray _img, std::vector<KeyPoint>& keypoints, int threshold)
{
    cv::Mat img;
    if(!_img.getMat().isContinuous())
      img = _img.getMat().clone();
    else
      img = _img.getMat();

    size_t total = 0;
    int xsize = img.cols;
    int ysize = img.rows;
    size_t nExpectedCorners = keypoints.capacity();
    register int x, y;
    register int xsizeB = xsize - 4;
    register int ysizeB = ysize - 3;
    register int width;

    keypoints.resize(0);

    int pixel_7_12d_[16];
    makeAgastOffsets(pixel_7_12d_, (int)img.step, AgastFeatureDetector::AGAST_7_12d);

    register short offset0 = (short) pixel_7_12d_[0];
    register short offset1 = (short) pixel_7_12d_[1];
    register short offset2 = (short) pixel_7_12d_[2];
    register short offset3 = (short) pixel_7_12d_[3];
    register short offset4 = (short) pixel_7_12d_[4];
    register short offset5 = (short) pixel_7_12d_[5];
    register short offset6 = (short) pixel_7_12d_[6];
    register short offset7 = (short) pixel_7_12d_[7];
    register short offset8 = (short) pixel_7_12d_[8];
    register short offset9 = (short) pixel_7_12d_[9];
    register short offset10 = (short) pixel_7_12d_[10];
    register short offset11 = (short) pixel_7_12d_[11];

    width = xsize;

    for(y = 3; y < ysizeB; y++)
    {
        x = 2;
        while(true)
        {
          homogeneous:
          {
            x++;
            if(x > xsizeB)
                break;
            else
            {
                register const unsigned char* const ptr = img.ptr() + y*width + x;
                register const int cb = *ptr + threshold;
                register const int c_b = *ptr - threshold;
                if(ptr[offset0] > cb)
                  if(ptr[offset5] > cb)
                    if(ptr[offset2] > cb)
                      if(ptr[offset9] > cb)
                        if(ptr[offset1] > cb)
                          if(ptr[offset6] > cb)
                            if(ptr[offset3] > cb)
                              if(ptr[offset4] > cb)
                                goto success_homogeneous;
                              else
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    goto success_structured;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset4] > cb)
                                      if(ptr[offset7] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset11] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset10] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset6] > cb)
                            if(ptr[offset7] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset3] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset6] > cb)
                                goto success_homogeneous;
                              else
                                if(ptr[offset11] > cb)
                                  goto success_homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset7] > cb)
                                  if(ptr[offset8] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                    else
                      if(ptr[offset9] > cb)
                        if(ptr[offset7] > cb)
                          if(ptr[offset8] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset10] > cb)
                                if(ptr[offset11] > cb)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset3] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        goto homogeneous;
                  else
                    if(ptr[offset5] < c_b)
                      if(ptr[offset9] > cb)
                        if(ptr[offset3] < c_b)
                          if(ptr[offset4] < c_b)
                            if(ptr[offset11] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset2] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset7] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset2] < c_b)
                                      if(ptr[offset7] < c_b)
                                        if(ptr[offset8] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset7] > cb)
                                    if(ptr[offset8] > cb)
                                      if(ptr[offset10] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset2] < c_b)
                                      if(ptr[offset7] < c_b)
                                        if(ptr[offset1] < c_b)
                                          goto success_structured;
                                        else
                                          if(ptr[offset8] < c_b)
                                            goto success_structured;
                                          else
                                            goto structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                            else
                              if(ptr[offset2] < c_b)
                                if(ptr[offset7] < c_b)
                                  if(ptr[offset1] < c_b)
                                    if(ptr[offset6] < c_b)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset8] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset11] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset1] > cb)
                                    if(ptr[offset2] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset7] > cb)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset11] > cb)
                            if(ptr[offset10] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset1] > cb)
                                  if(ptr[offset2] > cb)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset7] > cb)
                                      if(ptr[offset8] > cb)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset7] > cb)
                                      if(ptr[offset8] > cb)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset8] > cb)
                                  if(ptr[offset1] > cb)
                                    if(ptr[offset2] > cb)
                                      goto success_homogeneous;
                                    else
                                      if(ptr[offset7] > cb)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset9] < c_b)
                          if(ptr[offset2] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset4] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset3] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        if(ptr[offset11] < c_b)
                                          if(ptr[offset10] < c_b)
                                            goto success_structured;
                                          else
                                            goto structured;
                                        else
                                          goto structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset7] < c_b)
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset10] < c_b)
                                        if(ptr[offset4] < c_b)
                                          goto success_structured;
                                        else
                                          if(ptr[offset11] < c_b)
                                            goto success_structured;
                                          else
                                            goto structured;
                                      else
                                        if(ptr[offset3] < c_b)
                                          if(ptr[offset4] < c_b)
                                            goto success_structured;
                                          else
                                            goto structured;
                                        else
                                          goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset6] < c_b)
                                if(ptr[offset7] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset4] < c_b)
                                      if(ptr[offset3] < c_b)
                                        goto success_structured;
                                      else
                                        if(ptr[offset10] < c_b)
                                          goto success_structured;
                                        else
                                          goto homogeneous;
                                    else
                                      if(ptr[offset10] < c_b)
                                        if(ptr[offset11] < c_b)
                                          goto success_structured;
                                        else
                                          goto homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset6] < c_b)
                              if(ptr[offset7] < c_b)
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_homogeneous;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset2] < c_b)
                                    if(ptr[offset1] < c_b)
                                      if(ptr[offset3] < c_b)
                                        if(ptr[offset4] < c_b)
                                          goto success_structured;
                                        else
                                          goto homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset2] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            if(ptr[offset2] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset4] < c_b)
                                  if(ptr[offset7] < c_b)
                                    if(ptr[offset1] < c_b)
                                      if(ptr[offset6] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      if(ptr[offset6] < c_b)
                                        if(ptr[offset8] < c_b)
                                          goto success_homogeneous;
                                        else
                                          goto homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                    else
                      if(ptr[offset2] > cb)
                        if(ptr[offset10] > cb)
                          if(ptr[offset11] > cb)
                            if(ptr[offset9] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset8] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset7] > cb)
                                    if(ptr[offset8] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset4] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        if(ptr[offset9] > cb)
                          if(ptr[offset7] > cb)
                            if(ptr[offset8] > cb)
                              if(ptr[offset10] > cb)
                                if(ptr[offset11] > cb)
                                  if(ptr[offset1] > cb)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset6] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                else if(ptr[offset0] < c_b)
                  if(ptr[offset2] > cb)
                    if(ptr[offset5] > cb)
                      if(ptr[offset7] > cb)
                        if(ptr[offset6] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset3] > cb)
                              if(ptr[offset1] > cb)
                                goto success_homogeneous;
                              else
                                if(ptr[offset8] > cb)
                                  goto success_homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset9] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    goto success_structured;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset9] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    goto success_structured;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        if(ptr[offset9] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset10] < c_b)
                              if(ptr[offset11] < c_b)
                                if(ptr[offset7] < c_b)
                                  if(ptr[offset1] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset6] < c_b)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                    else
                      if(ptr[offset9] < c_b)
                        if(ptr[offset7] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset5] < c_b)
                              if(ptr[offset1] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset3] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset10] < c_b)
                                if(ptr[offset11] < c_b)
                                  if(ptr[offset1] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset6] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        goto homogeneous;
                  else
                    if(ptr[offset2] < c_b)
                      if(ptr[offset9] > cb)
                        if(ptr[offset5] > cb)
                          if(ptr[offset1] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset10] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset11] < c_b)
                                    goto success_structured;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset7] > cb)
                                    if(ptr[offset8] > cb)
                                      if(ptr[offset11] > cb)
                                        if(ptr[offset10] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset7] > cb)
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset4] > cb)
                                        goto success_structured;
                                      else
                                        if(ptr[offset11] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                    else
                                      if(ptr[offset3] > cb)
                                        if(ptr[offset4] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset6] > cb)
                              if(ptr[offset7] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset4] > cb)
                                    if(ptr[offset3] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] > cb)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset11] > cb)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset3] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset5] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset6] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset11] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset9] < c_b)
                          if(ptr[offset5] < c_b)
                            if(ptr[offset1] < c_b)
                              if(ptr[offset6] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset4] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        if(ptr[offset4] < c_b)
                                          if(ptr[offset7] < c_b)
                                            goto success_structured;
                                          else
                                            goto structured;
                                        else
                                          goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset11] < c_b)
                                  if(ptr[offset3] < c_b)
                                    if(ptr[offset4] < c_b)
                                      goto success_homogeneous;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset10] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset6] < c_b)
                                if(ptr[offset7] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset4] < c_b)
                                      if(ptr[offset3] < c_b)
                                        goto success_structured;
                                      else
                                        if(ptr[offset10] < c_b)
                                          goto success_structured;
                                        else
                                          goto homogeneous;
                                    else
                                      if(ptr[offset10] < c_b)
                                        if(ptr[offset11] < c_b)
                                          goto success_structured;
                                        else
                                          goto homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset10] < c_b)
                              if(ptr[offset11] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset3] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset8] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset3] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset5] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset6] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset11] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                    else
                      if(ptr[offset9] < c_b)
                        if(ptr[offset7] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset5] < c_b)
                              if(ptr[offset1] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset3] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_homogeneous;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset10] < c_b)
                                if(ptr[offset11] < c_b)
                                  if(ptr[offset1] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset6] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        if(ptr[offset5] > cb)
                          if(ptr[offset9] > cb)
                            if(ptr[offset6] > cb)
                              if(ptr[offset7] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset4] > cb)
                                    if(ptr[offset3] > cb)
                                      goto success_homogeneous;
                                    else
                                      if(ptr[offset10] > cb)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset11] > cb)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                else
                  if(ptr[offset5] > cb)
                    if(ptr[offset9] > cb)
                      if(ptr[offset6] > cb)
                        if(ptr[offset7] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset3] > cb)
                              if(ptr[offset8] > cb)
                                goto success_homogeneous;
                              else
                                if(ptr[offset1] > cb)
                                  if(ptr[offset2] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  goto success_homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset11] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  goto success_homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        goto homogeneous;
                    else
                      if(ptr[offset2] > cb)
                        if(ptr[offset3] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset7] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset6] > cb)
                                  goto success_homogeneous;
                                else
                                  goto homogeneous;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset8] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        goto homogeneous;
                  else
                    if(ptr[offset5] < c_b)
                      if(ptr[offset9] < c_b)
                        if(ptr[offset6] < c_b)
                          if(ptr[offset7] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset8] < c_b)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset1] < c_b)
                                    if(ptr[offset2] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset10] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset11] < c_b)
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset10] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        if(ptr[offset2] < c_b)
                          if(ptr[offset3] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset7] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset6] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset8] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                    else
                      goto homogeneous;
            }
          }
          structured:
          {
            x++;
            if(x > xsizeB)
                break;
            else
            {
                register const unsigned char* const ptr = img.ptr() + y*width + x;
                register const int cb = *ptr + threshold;
                register const int c_b = *ptr - threshold;
                if(ptr[offset0] > cb)
                  if(ptr[offset5] > cb)
                    if(ptr[offset2] > cb)
                      if(ptr[offset9] > cb)
                        if(ptr[offset1] > cb)
                          if(ptr[offset6] > cb)
                            if(ptr[offset3] > cb)
                              if(ptr[offset4] > cb)
                                goto success_structured;
                              else
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset4] > cb)
                                      if(ptr[offset7] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset11] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset10] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset6] > cb)
                            if(ptr[offset7] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                      else
                        if(ptr[offset3] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset6] > cb)
                                goto success_structured;
                              else
                                if(ptr[offset11] > cb)
                                  goto success_structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset7] > cb)
                                  if(ptr[offset8] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                    else
                      if(ptr[offset9] > cb)
                        if(ptr[offset7] > cb)
                          if(ptr[offset8] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset10] > cb)
                                if(ptr[offset11] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset3] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                      else
                        goto structured;
                  else
                    if(ptr[offset5] < c_b)
                      if(ptr[offset9] > cb)
                        if(ptr[offset3] < c_b)
                          if(ptr[offset4] < c_b)
                            if(ptr[offset11] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset2] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset7] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset2] < c_b)
                                      if(ptr[offset7] < c_b)
                                        if(ptr[offset8] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset7] > cb)
                                    if(ptr[offset8] > cb)
                                      if(ptr[offset10] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset2] < c_b)
                                      if(ptr[offset7] < c_b)
                                        if(ptr[offset1] < c_b)
                                          goto success_structured;
                                        else
                                          if(ptr[offset8] < c_b)
                                            goto success_structured;
                                          else
                                            goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                            else
                              if(ptr[offset2] < c_b)
                                if(ptr[offset7] < c_b)
                                  if(ptr[offset1] < c_b)
                                    if(ptr[offset6] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset8] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset11] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset1] > cb)
                                    if(ptr[offset2] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset7] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset11] > cb)
                            if(ptr[offset10] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset1] > cb)
                                  if(ptr[offset2] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset7] > cb)
                                      if(ptr[offset8] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset7] > cb)
                                      if(ptr[offset8] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset8] > cb)
                                  if(ptr[offset1] > cb)
                                    if(ptr[offset2] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset7] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                      else
                        if(ptr[offset9] < c_b)
                          if(ptr[offset2] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset4] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset3] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        if(ptr[offset11] < c_b)
                                          if(ptr[offset10] < c_b)
                                            goto success_structured;
                                          else
                                            goto structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset7] < c_b)
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset10] < c_b)
                                        if(ptr[offset4] < c_b)
                                          goto success_structured;
                                        else
                                          if(ptr[offset11] < c_b)
                                            goto success_structured;
                                          else
                                            goto structured;
                                      else
                                        if(ptr[offset3] < c_b)
                                          if(ptr[offset4] < c_b)
                                            goto success_structured;
                                          else
                                            goto structured;
                                        else
                                          goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset6] < c_b)
                                if(ptr[offset7] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset4] < c_b)
                                      if(ptr[offset3] < c_b)
                                        goto success_structured;
                                      else
                                        if(ptr[offset10] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        if(ptr[offset11] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset6] < c_b)
                              if(ptr[offset7] < c_b)
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset2] < c_b)
                                    if(ptr[offset1] < c_b)
                                      if(ptr[offset3] < c_b)
                                        if(ptr[offset4] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset2] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            if(ptr[offset2] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset4] < c_b)
                                  if(ptr[offset7] < c_b)
                                    if(ptr[offset1] < c_b)
                                      if(ptr[offset6] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      if(ptr[offset6] < c_b)
                                        if(ptr[offset8] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto homogeneous;
                    else
                      if(ptr[offset2] > cb)
                        if(ptr[offset10] > cb)
                          if(ptr[offset11] > cb)
                            if(ptr[offset9] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset8] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset7] > cb)
                                    if(ptr[offset8] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset4] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                      else
                        if(ptr[offset9] > cb)
                          if(ptr[offset7] > cb)
                            if(ptr[offset8] > cb)
                              if(ptr[offset10] > cb)
                                if(ptr[offset11] > cb)
                                  if(ptr[offset1] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset6] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                else if(ptr[offset0] < c_b)
                  if(ptr[offset2] > cb)
                    if(ptr[offset5] > cb)
                      if(ptr[offset7] > cb)
                        if(ptr[offset6] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset3] > cb)
                              if(ptr[offset1] > cb)
                                goto success_structured;
                              else
                                if(ptr[offset8] > cb)
                                  goto success_structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset9] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset9] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          goto structured;
                      else
                        if(ptr[offset9] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset10] < c_b)
                              if(ptr[offset11] < c_b)
                                if(ptr[offset7] < c_b)
                                  if(ptr[offset1] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset6] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                    else
                      if(ptr[offset9] < c_b)
                        if(ptr[offset7] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset5] < c_b)
                              if(ptr[offset1] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset3] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset10] < c_b)
                                if(ptr[offset11] < c_b)
                                  if(ptr[offset1] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset6] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                      else
                        goto structured;
                  else
                    if(ptr[offset2] < c_b)
                      if(ptr[offset9] > cb)
                        if(ptr[offset5] > cb)
                          if(ptr[offset1] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset10] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset11] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset7] > cb)
                                    if(ptr[offset8] > cb)
                                      if(ptr[offset11] > cb)
                                        if(ptr[offset10] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset7] > cb)
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset4] > cb)
                                        goto success_structured;
                                      else
                                        if(ptr[offset11] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                    else
                                      if(ptr[offset3] > cb)
                                        if(ptr[offset4] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset6] > cb)
                              if(ptr[offset7] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset4] > cb)
                                    if(ptr[offset3] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset11] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset3] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset5] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset6] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset11] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                      else
                        if(ptr[offset9] < c_b)
                          if(ptr[offset5] < c_b)
                            if(ptr[offset1] < c_b)
                              if(ptr[offset6] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset4] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        if(ptr[offset4] < c_b)
                                          if(ptr[offset7] < c_b)
                                            goto success_structured;
                                          else
                                            goto structured;
                                        else
                                          goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset11] < c_b)
                                  if(ptr[offset3] < c_b)
                                    if(ptr[offset4] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset6] < c_b)
                                if(ptr[offset7] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset4] < c_b)
                                      if(ptr[offset3] < c_b)
                                        goto success_structured;
                                      else
                                        if(ptr[offset10] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        if(ptr[offset11] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset10] < c_b)
                              if(ptr[offset11] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset3] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset8] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset3] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset5] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset6] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset11] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                    else
                      if(ptr[offset9] < c_b)
                        if(ptr[offset7] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset5] < c_b)
                              if(ptr[offset1] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset3] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset10] < c_b)
                                if(ptr[offset11] < c_b)
                                  if(ptr[offset1] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset6] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                      else
                        if(ptr[offset5] > cb)
                          if(ptr[offset9] > cb)
                            if(ptr[offset6] > cb)
                              if(ptr[offset7] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset4] > cb)
                                    if(ptr[offset3] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset11] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto homogeneous;
                        else
                          goto structured;
                else
                  if(ptr[offset5] > cb)
                    if(ptr[offset9] > cb)
                      if(ptr[offset6] > cb)
                        if(ptr[offset7] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset3] > cb)
                              if(ptr[offset8] > cb)
                                goto success_structured;
                              else
                                if(ptr[offset1] > cb)
                                  if(ptr[offset2] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset11] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          goto structured;
                      else
                        goto structured;
                    else
                      if(ptr[offset2] > cb)
                        if(ptr[offset3] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset7] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset6] > cb)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset8] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                      else
                        goto structured;
                  else
                    if(ptr[offset5] < c_b)
                      if(ptr[offset9] < c_b)
                        if(ptr[offset6] < c_b)
                          if(ptr[offset7] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset8] < c_b)
                                  goto success_structured;
                                else
                                  if(ptr[offset1] < c_b)
                                    if(ptr[offset2] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset10] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset11] < c_b)
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset10] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                      else
                        if(ptr[offset2] < c_b)
                          if(ptr[offset3] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset7] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset6] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset8] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                    else
                      goto homogeneous;
            }
          }
          success_homogeneous:
            if(total == nExpectedCorners)
            {
                if(nExpectedCorners == 0)
                {
                    nExpectedCorners = 512;
                    keypoints.reserve(nExpectedCorners);
                }
                else
                {
                    nExpectedCorners *= 2;
                    keypoints.reserve(nExpectedCorners);
                }
            }
            keypoints.push_back(KeyPoint(Point2f((float)x, (float)y), 1.0f));
            total++;
            goto homogeneous;
          success_structured:
            if(total == nExpectedCorners)
            {
                if(nExpectedCorners == 0)
                {
                    nExpectedCorners = 512;
                    keypoints.reserve(nExpectedCorners);
                }
                else
                {
                    nExpectedCorners *= 2;
                    keypoints.reserve(nExpectedCorners);
                }
            }
            keypoints.push_back(KeyPoint(Point2f((float)x, (float)y), 1.0f));
            total++;
            goto structured;
        }
    }
}

static void AGAST_7_12s(InputArray _img, std::vector<KeyPoint>& keypoints, int threshold)
{
    cv::Mat img;
    if(!_img.getMat().isContinuous())
      img = _img.getMat().clone();
    else
      img = _img.getMat();

    size_t total = 0;
    int xsize = img.cols;
    int ysize = img.rows;
    size_t nExpectedCorners = keypoints.capacity();
    register int x, y;
    register int xsizeB=xsize - 3; //2, +1 due to faster test x>xsizeB
    register int ysizeB=ysize - 2;
    register int width;

    keypoints.resize(0);

    int pixel_7_12s_[16];
    makeAgastOffsets(pixel_7_12s_, (int)img.step, AgastFeatureDetector::AGAST_7_12s);

    register short offset0 = (short) pixel_7_12s_[0];
    register short offset1 = (short) pixel_7_12s_[1];
    register short offset2 = (short) pixel_7_12s_[2];
    register short offset3 = (short) pixel_7_12s_[3];
    register short offset4 = (short) pixel_7_12s_[4];
    register short offset5 = (short) pixel_7_12s_[5];
    register short offset6 = (short) pixel_7_12s_[6];
    register short offset7 = (short) pixel_7_12s_[7];
    register short offset8 = (short) pixel_7_12s_[8];
    register short offset9 = (short) pixel_7_12s_[9];
    register short offset10 = (short) pixel_7_12s_[10];
    register short offset11 = (short) pixel_7_12s_[11];

    width = xsize;

    for(y = 2; y < ysizeB; y++)
    {
        x = 1;
        while(true)
        {
          homogeneous:
          {
            x++;
            if(x > xsizeB)
                break;
            else
            {
                register const unsigned char* const ptr = img.ptr() + y*width + x;
                register const int cb = *ptr + threshold;
                register const int c_b = *ptr - threshold;
                if(ptr[offset0] > cb)
                  if(ptr[offset2] > cb)
                    if(ptr[offset5] > cb)
                      if(ptr[offset9] > cb)
                        if(ptr[offset7] > cb)
                          if(ptr[offset1] > cb)
                            if(ptr[offset6] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset11] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset11] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset4] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset6] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset1] > cb)
                            if(ptr[offset11] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset10] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset4] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset3] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset7] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset6] > cb)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset11] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset8] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset1] > cb)
                                if(ptr[offset6] > cb)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset11] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                    else
                      if(ptr[offset9] < c_b)
                        if(ptr[offset7] < c_b)
                          if(ptr[offset5] < c_b)
                            if(ptr[offset1] > cb)
                              if(ptr[offset4] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset3] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset11] < c_b)
                                        if(ptr[offset10] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        if(ptr[offset11] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                    else
                                      if(ptr[offset3] < c_b)
                                        if(ptr[offset4] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset6] < c_b)
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset1] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset1] > cb)
                            if(ptr[offset3] > cb)
                              if(ptr[offset4] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset10] > cb)
                          if(ptr[offset11] > cb)
                            if(ptr[offset9] > cb)
                              if(ptr[offset7] > cb)
                                if(ptr[offset1] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset8] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset8] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset1] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset8] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset4] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                  else
                    if(ptr[offset7] > cb)
                      if(ptr[offset9] > cb)
                        if(ptr[offset8] > cb)
                          if(ptr[offset5] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset10] > cb)
                                if(ptr[offset11] > cb)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset3] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset10] > cb)
                              if(ptr[offset11] > cb)
                                if(ptr[offset1] > cb)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset6] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        goto homogeneous;
                    else
                      if(ptr[offset7] < c_b)
                        if(ptr[offset5] < c_b)
                          if(ptr[offset2] < c_b)
                            if(ptr[offset6] < c_b)
                              if(ptr[offset4] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset1] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset8] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset9] < c_b)
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset9] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            if(ptr[offset9] < c_b)
                              if(ptr[offset6] < c_b)
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_homogeneous;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        goto homogeneous;
                else if(ptr[offset0] < c_b)
                  if(ptr[offset2] < c_b)
                    if(ptr[offset9] < c_b)
                      if(ptr[offset5] < c_b)
                        if(ptr[offset7] < c_b)
                          if(ptr[offset1] < c_b)
                            if(ptr[offset6] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset4] < c_b)
                                  goto success_structured;
                                else
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset4] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset11] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset4] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] < c_b)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset10] < c_b)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset6] < c_b)
                              if(ptr[offset8] < c_b)
                                if(ptr[offset4] < c_b)
                                  if(ptr[offset3] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] < c_b)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset1] < c_b)
                            if(ptr[offset11] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset4] < c_b)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset10] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset10] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset6] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset4] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset10] < c_b)
                          if(ptr[offset11] < c_b)
                            if(ptr[offset7] < c_b)
                              if(ptr[offset1] < c_b)
                                if(ptr[offset3] < c_b)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset8] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset8] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset1] < c_b)
                                if(ptr[offset3] < c_b)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset8] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                    else
                      if(ptr[offset9] > cb)
                        if(ptr[offset5] > cb)
                          if(ptr[offset7] > cb)
                            if(ptr[offset1] < c_b)
                              if(ptr[offset4] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset3] < c_b)
                                    if(ptr[offset11] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto homogeneous;
                                else
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset8] > cb)
                                      if(ptr[offset11] > cb)
                                        if(ptr[offset10] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset4] > cb)
                                        goto success_structured;
                                      else
                                        if(ptr[offset11] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                    else
                                      if(ptr[offset3] > cb)
                                        if(ptr[offset4] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset4] > cb)
                                    if(ptr[offset3] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] > cb)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset11] > cb)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset1] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset4] < c_b)
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          if(ptr[offset3] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset5] < c_b)
                                if(ptr[offset7] < c_b)
                                  if(ptr[offset1] < c_b)
                                    if(ptr[offset6] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset8] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset1] < c_b)
                                    if(ptr[offset6] < c_b)
                                      goto success_homogeneous;
                                    else
                                      if(ptr[offset11] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        if(ptr[offset3] < c_b)
                          if(ptr[offset4] < c_b)
                            if(ptr[offset5] < c_b)
                              if(ptr[offset7] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset6] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset11] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset8] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset6] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset11] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset1] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                  else
                    if(ptr[offset7] > cb)
                      if(ptr[offset5] > cb)
                        if(ptr[offset2] > cb)
                          if(ptr[offset6] > cb)
                            if(ptr[offset4] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset1] > cb)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset8] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset9] > cb)
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset9] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          if(ptr[offset9] > cb)
                            if(ptr[offset6] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        goto homogeneous;
                    else
                      if(ptr[offset7] < c_b)
                        if(ptr[offset9] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset5] < c_b)
                              if(ptr[offset1] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset3] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_homogeneous;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_homogeneous;
                                      else
                                        goto homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset10] < c_b)
                                if(ptr[offset11] < c_b)
                                  if(ptr[offset1] < c_b)
                                    goto success_homogeneous;
                                  else
                                    if(ptr[offset6] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        goto homogeneous;
                else
                  if(ptr[offset5] > cb)
                    if(ptr[offset7] > cb)
                      if(ptr[offset9] > cb)
                        if(ptr[offset6] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset3] > cb)
                              if(ptr[offset8] > cb)
                                goto success_homogeneous;
                              else
                                if(ptr[offset1] > cb)
                                  if(ptr[offset2] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  goto success_homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            if(ptr[offset11] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  goto success_homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                        else
                          goto homogeneous;
                      else
                        if(ptr[offset2] > cb)
                          if(ptr[offset3] > cb)
                            if(ptr[offset4] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset6] > cb)
                                  goto success_homogeneous;
                                else
                                  goto homogeneous;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset8] > cb)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          goto homogeneous;
                    else
                      goto homogeneous;
                  else
                    if(ptr[offset5] < c_b)
                      if(ptr[offset7] < c_b)
                        if(ptr[offset9] < c_b)
                          if(ptr[offset6] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset8] < c_b)
                                  goto success_homogeneous;
                                else
                                  if(ptr[offset1] < c_b)
                                    if(ptr[offset2] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset10] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                            else
                              if(ptr[offset11] < c_b)
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset10] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  goto homogeneous;
                              else
                                goto homogeneous;
                          else
                            goto homogeneous;
                        else
                          if(ptr[offset2] < c_b)
                            if(ptr[offset3] < c_b)
                              if(ptr[offset4] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset6] < c_b)
                                    goto success_homogeneous;
                                  else
                                    goto homogeneous;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset8] < c_b)
                                      goto success_homogeneous;
                                    else
                                      goto homogeneous;
                                  else
                                    goto homogeneous;
                              else
                                goto homogeneous;
                            else
                              goto homogeneous;
                          else
                            goto homogeneous;
                      else
                        goto homogeneous;
                    else
                      goto homogeneous;
            }
          }
          structured:
          {
            x++;
            if(x > xsizeB)
                break;
            else
            {
                register const unsigned char* const ptr = img.ptr() + y*width + x;
                register const int cb = *ptr + threshold;
                register const int c_b = *ptr - threshold;
                if(ptr[offset0] > cb)
                  if(ptr[offset2] > cb)
                    if(ptr[offset5] > cb)
                      if(ptr[offset9] > cb)
                        if(ptr[offset7] > cb)
                          if(ptr[offset1] > cb)
                            if(ptr[offset6] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset11] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset11] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset4] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset6] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset1] > cb)
                            if(ptr[offset11] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset10] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset4] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                      else
                        if(ptr[offset3] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset7] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset6] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset11] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset8] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset1] > cb)
                                if(ptr[offset6] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset11] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                    else
                      if(ptr[offset7] < c_b)
                        if(ptr[offset9] < c_b)
                          if(ptr[offset5] < c_b)
                            if(ptr[offset1] > cb)
                              if(ptr[offset4] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset3] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset11] < c_b)
                                        if(ptr[offset10] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        if(ptr[offset11] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                    else
                                      if(ptr[offset3] < c_b)
                                        if(ptr[offset4] < c_b)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset6] < c_b)
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset1] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset10] > cb)
                            if(ptr[offset11] > cb)
                              if(ptr[offset9] > cb)
                                if(ptr[offset1] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset8] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                if(ptr[offset1] > cb)
                                  if(ptr[offset3] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                      else
                        if(ptr[offset10] > cb)
                          if(ptr[offset11] > cb)
                            if(ptr[offset9] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset8] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset7] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset4] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                  else
                    if(ptr[offset7] > cb)
                      if(ptr[offset9] > cb)
                        if(ptr[offset8] > cb)
                          if(ptr[offset5] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset10] > cb)
                                if(ptr[offset11] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset3] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset6] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset10] > cb)
                              if(ptr[offset11] > cb)
                                if(ptr[offset1] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset6] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          goto structured;
                      else
                        goto structured;
                    else
                      if(ptr[offset7] < c_b)
                        if(ptr[offset5] < c_b)
                          if(ptr[offset2] < c_b)
                            if(ptr[offset6] < c_b)
                              if(ptr[offset4] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset1] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset8] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset9] < c_b)
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset9] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                          else
                            if(ptr[offset9] < c_b)
                              if(ptr[offset6] < c_b)
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          goto structured;
                      else
                        goto structured;
                else if(ptr[offset0] < c_b)
                  if(ptr[offset2] < c_b)
                    if(ptr[offset11] < c_b)
                      if(ptr[offset3] < c_b)
                        if(ptr[offset5] < c_b)
                          if(ptr[offset9] < c_b)
                            if(ptr[offset7] < c_b)
                              if(ptr[offset1] < c_b)
                                if(ptr[offset4] < c_b)
                                  goto success_structured;
                                else
                                  if(ptr[offset10] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset4] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset1] < c_b)
                                if(ptr[offset4] < c_b)
                                  goto success_structured;
                                else
                                  if(ptr[offset10] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset4] < c_b)
                              if(ptr[offset7] < c_b)
                                if(ptr[offset1] < c_b)
                                  goto success_structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset8] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset1] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset10] < c_b)
                            if(ptr[offset9] < c_b)
                              if(ptr[offset7] < c_b)
                                if(ptr[offset1] < c_b)
                                  goto success_structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset8] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset1] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset1] < c_b)
                                if(ptr[offset4] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset7] > cb)
                              if(ptr[offset9] > cb)
                                if(ptr[offset5] > cb)
                                  if(ptr[offset4] > cb)
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset8] > cb)
                                        if(ptr[offset10] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                      else
                        if(ptr[offset9] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset10] < c_b)
                              if(ptr[offset7] < c_b)
                                if(ptr[offset1] < c_b)
                                  goto success_structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset1] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                        else
                          if(ptr[offset5] > cb)
                            if(ptr[offset7] > cb)
                              if(ptr[offset9] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset8] > cb)
                                      if(ptr[offset3] > cb)
                                        goto success_structured;
                                      else
                                        if(ptr[offset10] > cb)
                                          goto success_structured;
                                        else
                                          goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                    else
                      if(ptr[offset4] < c_b)
                        if(ptr[offset5] < c_b)
                          if(ptr[offset7] < c_b)
                            if(ptr[offset6] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset1] < c_b)
                                  goto success_structured;
                                else
                                  if(ptr[offset8] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset9] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset10] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                          else
                            if(ptr[offset1] < c_b)
                              if(ptr[offset6] < c_b)
                                if(ptr[offset3] < c_b)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          if(ptr[offset7] > cb)
                            if(ptr[offset9] > cb)
                              if(ptr[offset5] > cb)
                                if(ptr[offset6] > cb)
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset11] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                      else
                        if(ptr[offset5] > cb)
                          if(ptr[offset7] > cb)
                            if(ptr[offset9] > cb)
                              if(ptr[offset6] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset4] > cb)
                                      goto success_structured;
                                    else
                                      if(ptr[offset11] > cb)
                                        goto success_structured;
                                      else
                                        goto homogeneous;
                                  else
                                    if(ptr[offset3] > cb)
                                      if(ptr[offset4] > cb)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                  else
                    if(ptr[offset7] > cb)
                      if(ptr[offset5] > cb)
                        if(ptr[offset2] > cb)
                          if(ptr[offset6] > cb)
                            if(ptr[offset4] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset1] > cb)
                                  goto success_structured;
                                else
                                  if(ptr[offset8] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset9] > cb)
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset9] > cb)
                                if(ptr[offset8] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          if(ptr[offset9] > cb)
                            if(ptr[offset6] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset3] > cb)
                                    goto success_structured;
                                  else
                                    if(ptr[offset10] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                      else
                        goto structured;
                    else
                      if(ptr[offset7] < c_b)
                        if(ptr[offset9] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset5] < c_b)
                              if(ptr[offset1] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset3] < c_b)
                                      if(ptr[offset4] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      goto success_structured;
                                    else
                                      if(ptr[offset10] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        goto success_structured;
                                      else
                                        goto structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset10] < c_b)
                                if(ptr[offset11] < c_b)
                                  if(ptr[offset1] < c_b)
                                    goto success_structured;
                                  else
                                    if(ptr[offset6] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                      else
                        goto structured;
                else
                  if(ptr[offset5] > cb)
                    if(ptr[offset7] > cb)
                      if(ptr[offset9] > cb)
                        if(ptr[offset6] > cb)
                          if(ptr[offset4] > cb)
                            if(ptr[offset3] > cb)
                              if(ptr[offset8] > cb)
                                goto success_structured;
                              else
                                if(ptr[offset1] > cb)
                                  if(ptr[offset2] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            if(ptr[offset11] > cb)
                              if(ptr[offset8] > cb)
                                if(ptr[offset10] > cb)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                        else
                          goto structured;
                      else
                        if(ptr[offset2] > cb)
                          if(ptr[offset3] > cb)
                            if(ptr[offset4] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset6] > cb)
                                  goto success_structured;
                                else
                                  goto structured;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset8] > cb)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                        else
                          goto structured;
                    else
                      goto structured;
                  else
                    if(ptr[offset5] < c_b)
                      if(ptr[offset7] < c_b)
                        if(ptr[offset9] < c_b)
                          if(ptr[offset6] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset8] < c_b)
                                  goto success_structured;
                                else
                                  if(ptr[offset1] < c_b)
                                    if(ptr[offset2] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset10] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                            else
                              if(ptr[offset11] < c_b)
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset10] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  goto structured;
                              else
                                goto structured;
                          else
                            goto structured;
                        else
                          if(ptr[offset2] < c_b)
                            if(ptr[offset3] < c_b)
                              if(ptr[offset4] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset6] < c_b)
                                    goto success_structured;
                                  else
                                    goto structured;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset8] < c_b)
                                      goto success_structured;
                                    else
                                      goto structured;
                                  else
                                    goto structured;
                              else
                                goto structured;
                            else
                              goto structured;
                          else
                            goto structured;
                      else
                        goto structured;
                    else
                      goto homogeneous;
            }
          }
          success_homogeneous:
            if(total == nExpectedCorners)
            {
                if(nExpectedCorners == 0)
                {
                    nExpectedCorners = 512;
                    keypoints.reserve(nExpectedCorners);
                }
                else
                {
                    nExpectedCorners *= 2;
                    keypoints.reserve(nExpectedCorners);
                }
            }
            keypoints.push_back(KeyPoint(Point2f((float)x, (float)y), 1.0f));
            total++;
            goto homogeneous;
          success_structured:
            if(total == nExpectedCorners)
            {
                if(nExpectedCorners == 0)
                {
                    nExpectedCorners = 512;
                    keypoints.reserve(nExpectedCorners);
                }
                else
                {
                    nExpectedCorners *= 2;
                    keypoints.reserve(nExpectedCorners);
                }
            }
            keypoints.push_back(KeyPoint(Point2f((float)x, (float)y), 1.0f));
            total++;
            goto structured;
        }
    }
}

static void OAST_9_16(InputArray _img, std::vector<KeyPoint>& keypoints, int threshold)
{
    cv::Mat img;
    if(!_img.getMat().isContinuous())
      img = _img.getMat().clone();
    else
      img = _img.getMat();

    size_t total = 0;
    int xsize = img.cols;
    int ysize = img.rows;
    size_t nExpectedCorners = keypoints.capacity();
    register int x, y;
    register int xsizeB=xsize - 4;
    register int ysizeB=ysize - 3;
    register int width;

    keypoints.resize(0);

    int pixel_9_16_[16];
    makeAgastOffsets(pixel_9_16_, (int)img.step, AgastFeatureDetector::OAST_9_16);

    register short offset0 = (short) pixel_9_16_[0];
    register short offset1 = (short) pixel_9_16_[1];
    register short offset2 = (short) pixel_9_16_[2];
    register short offset3 = (short) pixel_9_16_[3];
    register short offset4 = (short) pixel_9_16_[4];
    register short offset5 = (short) pixel_9_16_[5];
    register short offset6 = (short) pixel_9_16_[6];
    register short offset7 = (short) pixel_9_16_[7];
    register short offset8 = (short) pixel_9_16_[8];
    register short offset9 = (short) pixel_9_16_[9];
    register short offset10 = (short) pixel_9_16_[10];
    register short offset11 = (short) pixel_9_16_[11];
    register short offset12 = (short) pixel_9_16_[12];
    register short offset13 = (short) pixel_9_16_[13];
    register short offset14 = (short) pixel_9_16_[14];
    register short offset15 = (short) pixel_9_16_[15];

    width = xsize;

    for(y = 3; y < ysizeB; y++)
    {
        x = 2;
        while(true)
        {
            x++;
            if(x > xsizeB)
                break;
            else
            {
                register const unsigned char* const ptr = img.ptr() + y*width + x;
                register const int cb = *ptr + threshold;
                register const int c_b = *ptr - threshold;
                if(ptr[offset0] > cb)
                  if(ptr[offset2] > cb)
                    if(ptr[offset4] > cb)
                      if(ptr[offset5] > cb)
                        if(ptr[offset7] > cb)
                          if(ptr[offset3] > cb)
                            if(ptr[offset1] > cb)
                              if(ptr[offset6] > cb)
                                if(ptr[offset8] > cb)
                                  {}
                                else
                                  if(ptr[offset15] > cb)
                                    {}
                                  else
                                    continue;
                              else
                                if(ptr[offset13] > cb)
                                  if(ptr[offset14] > cb)
                                    if(ptr[offset15] > cb)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset8] > cb)
                                if(ptr[offset9] > cb)
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset6] > cb)
                                      {}
                                    else
                                      if(ptr[offset11] > cb)
                                        if(ptr[offset12] > cb)
                                          if(ptr[offset13] > cb)
                                            if(ptr[offset14] > cb)
                                              if(ptr[offset15] > cb)
                                                {}
                                              else
                                                continue;
                                            else
                                              continue;
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            if(ptr[offset10] > cb)
                              if(ptr[offset11] > cb)
                                if(ptr[offset12] > cb)
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset9] > cb)
                                      if(ptr[offset6] > cb)
                                        {}
                                      else
                                        if(ptr[offset13] > cb)
                                          if(ptr[offset14] > cb)
                                            if(ptr[offset15] > cb)
                                              {}
                                            else
                                              continue;
                                          else
                                            continue;
                                        else
                                          continue;
                                    else
                                      if(ptr[offset1] > cb)
                                        if(ptr[offset13] > cb)
                                          if(ptr[offset14] > cb)
                                            if(ptr[offset15] > cb)
                                              {}
                                            else
                                              continue;
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                  else
                                    if(ptr[offset1] > cb)
                                      if(ptr[offset13] > cb)
                                        if(ptr[offset14] > cb)
                                          if(ptr[offset15] > cb)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                        else if(ptr[offset7] < c_b)
                          if(ptr[offset14] > cb)
                            if(ptr[offset15] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset6] > cb)
                                    {}
                                  else
                                    if(ptr[offset13] > cb)
                                      {}
                                    else
                                      continue;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      if(ptr[offset12] > cb)
                                        if(ptr[offset13] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset8] > cb)
                                  if(ptr[offset9] > cb)
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset11] > cb)
                                        if(ptr[offset12] > cb)
                                          if(ptr[offset13] > cb)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              continue;
                          else if(ptr[offset14] < c_b)
                            if(ptr[offset8] < c_b)
                              if(ptr[offset9] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    if(ptr[offset12] < c_b)
                                      if(ptr[offset13] < c_b)
                                        if(ptr[offset6] < c_b)
                                          {}
                                        else
                                          if(ptr[offset15] < c_b)
                                            {}
                                          else
                                            continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          if(ptr[offset14] > cb)
                            if(ptr[offset15] > cb)
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset6] > cb)
                                    {}
                                  else
                                    if(ptr[offset13] > cb)
                                      {}
                                    else
                                      continue;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      if(ptr[offset12] > cb)
                                        if(ptr[offset13] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset8] > cb)
                                  if(ptr[offset9] > cb)
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset11] > cb)
                                        if(ptr[offset12] > cb)
                                          if(ptr[offset13] > cb)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              continue;
                          else
                            continue;
                      else if(ptr[offset5] < c_b)
                        if(ptr[offset12] > cb)
                          if(ptr[offset13] > cb)
                            if(ptr[offset14] > cb)
                              if(ptr[offset15] > cb)
                                if(ptr[offset1] > cb)
                                  if(ptr[offset3] > cb)
                                    {}
                                  else
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset11] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset9] > cb)
                                      if(ptr[offset10] > cb)
                                        if(ptr[offset11] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset7] > cb)
                                    if(ptr[offset8] > cb)
                                      if(ptr[offset9] > cb)
                                        if(ptr[offset10] > cb)
                                          if(ptr[offset11] > cb)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              continue;
                          else
                            continue;
                        else if(ptr[offset12] < c_b)
                          if(ptr[offset7] < c_b)
                            if(ptr[offset8] < c_b)
                              if(ptr[offset9] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    if(ptr[offset13] < c_b)
                                      if(ptr[offset6] < c_b)
                                        {}
                                      else
                                        if(ptr[offset14] < c_b)
                                          if(ptr[offset15] < c_b)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          continue;
                      else
                        if(ptr[offset12] > cb)
                          if(ptr[offset13] > cb)
                            if(ptr[offset14] > cb)
                              if(ptr[offset15] > cb)
                                if(ptr[offset1] > cb)
                                  if(ptr[offset3] > cb)
                                    {}
                                  else
                                    if(ptr[offset10] > cb)
                                      if(ptr[offset11] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset8] > cb)
                                    if(ptr[offset9] > cb)
                                      if(ptr[offset10] > cb)
                                        if(ptr[offset11] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset6] > cb)
                                  if(ptr[offset7] > cb)
                                    if(ptr[offset8] > cb)
                                      if(ptr[offset9] > cb)
                                        if(ptr[offset10] > cb)
                                          if(ptr[offset11] > cb)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              continue;
                          else
                            continue;
                        else if(ptr[offset12] < c_b)
                          if(ptr[offset7] < c_b)
                            if(ptr[offset8] < c_b)
                              if(ptr[offset9] < c_b)
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    if(ptr[offset13] < c_b)
                                      if(ptr[offset14] < c_b)
                                        if(ptr[offset6] < c_b)
                                          {}
                                        else
                                          if(ptr[offset15] < c_b)
                                            {}
                                          else
                                            continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          continue;
                    else if(ptr[offset4] < c_b)
                      if(ptr[offset11] > cb)
                        if(ptr[offset12] > cb)
                          if(ptr[offset13] > cb)
                            if(ptr[offset10] > cb)
                              if(ptr[offset14] > cb)
                                if(ptr[offset15] > cb)
                                  if(ptr[offset1] > cb)
                                    {}
                                  else
                                    if(ptr[offset8] > cb)
                                      if(ptr[offset9] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset7] > cb)
                                      if(ptr[offset8] > cb)
                                        if(ptr[offset9] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset5] > cb)
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset7] > cb)
                                      if(ptr[offset8] > cb)
                                        if(ptr[offset9] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset14] > cb)
                                    if(ptr[offset15] > cb)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            continue;
                        else
                          continue;
                      else if(ptr[offset11] < c_b)
                        if(ptr[offset7] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset9] < c_b)
                              if(ptr[offset10] < c_b)
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset5] < c_b)
                                    if(ptr[offset3] < c_b)
                                      {}
                                    else
                                      if(ptr[offset12] < c_b)
                                        {}
                                      else
                                        continue;
                                  else
                                    if(ptr[offset12] < c_b)
                                      if(ptr[offset13] < c_b)
                                        if(ptr[offset14] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset12] < c_b)
                                    if(ptr[offset13] < c_b)
                                      if(ptr[offset14] < c_b)
                                        if(ptr[offset15] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          continue;
                      else
                        continue;
                    else
                      if(ptr[offset11] > cb)
                        if(ptr[offset12] > cb)
                          if(ptr[offset13] > cb)
                            if(ptr[offset10] > cb)
                              if(ptr[offset14] > cb)
                                if(ptr[offset15] > cb)
                                  if(ptr[offset1] > cb)
                                    {}
                                  else
                                    if(ptr[offset8] > cb)
                                      if(ptr[offset9] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset7] > cb)
                                      if(ptr[offset8] > cb)
                                        if(ptr[offset9] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset5] > cb)
                                  if(ptr[offset6] > cb)
                                    if(ptr[offset7] > cb)
                                      if(ptr[offset8] > cb)
                                        if(ptr[offset9] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset1] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset14] > cb)
                                    if(ptr[offset15] > cb)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            continue;
                        else
                          continue;
                      else if(ptr[offset11] < c_b)
                        if(ptr[offset7] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset9] < c_b)
                              if(ptr[offset10] < c_b)
                                if(ptr[offset12] < c_b)
                                  if(ptr[offset13] < c_b)
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset5] < c_b)
                                        {}
                                      else
                                        if(ptr[offset14] < c_b)
                                          {}
                                        else
                                          continue;
                                    else
                                      if(ptr[offset14] < c_b)
                                        if(ptr[offset15] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          continue;
                      else
                        continue;
                  else if(ptr[offset2] < c_b)
                    if(ptr[offset9] > cb)
                      if(ptr[offset10] > cb)
                        if(ptr[offset11] > cb)
                          if(ptr[offset8] > cb)
                            if(ptr[offset12] > cb)
                              if(ptr[offset13] > cb)
                                if(ptr[offset14] > cb)
                                  if(ptr[offset15] > cb)
                                    {}
                                  else
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset5] > cb)
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset4] > cb)
                                  if(ptr[offset5] > cb)
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset5] > cb)
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            if(ptr[offset1] > cb)
                              if(ptr[offset12] > cb)
                                if(ptr[offset13] > cb)
                                  if(ptr[offset14] > cb)
                                    if(ptr[offset15] > cb)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                        else
                          continue;
                      else
                        continue;
                    else if(ptr[offset9] < c_b)
                      if(ptr[offset7] < c_b)
                        if(ptr[offset8] < c_b)
                          if(ptr[offset6] < c_b)
                            if(ptr[offset5] < c_b)
                              if(ptr[offset4] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset1] < c_b)
                                    {}
                                  else
                                    if(ptr[offset10] < c_b)
                                      {}
                                    else
                                      continue;
                                else
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      if(ptr[offset12] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    if(ptr[offset12] < c_b)
                                      if(ptr[offset13] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset10] < c_b)
                                if(ptr[offset11] < c_b)
                                  if(ptr[offset12] < c_b)
                                    if(ptr[offset13] < c_b)
                                      if(ptr[offset14] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            if(ptr[offset10] < c_b)
                              if(ptr[offset11] < c_b)
                                if(ptr[offset12] < c_b)
                                  if(ptr[offset13] < c_b)
                                    if(ptr[offset14] < c_b)
                                      if(ptr[offset15] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                        else
                          continue;
                      else
                        continue;
                    else
                      continue;
                  else
                    if(ptr[offset9] > cb)
                      if(ptr[offset10] > cb)
                        if(ptr[offset11] > cb)
                          if(ptr[offset8] > cb)
                            if(ptr[offset12] > cb)
                              if(ptr[offset13] > cb)
                                if(ptr[offset14] > cb)
                                  if(ptr[offset15] > cb)
                                    {}
                                  else
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset5] > cb)
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset4] > cb)
                                  if(ptr[offset5] > cb)
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset3] > cb)
                                if(ptr[offset4] > cb)
                                  if(ptr[offset5] > cb)
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset7] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            if(ptr[offset1] > cb)
                              if(ptr[offset12] > cb)
                                if(ptr[offset13] > cb)
                                  if(ptr[offset14] > cb)
                                    if(ptr[offset15] > cb)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                        else
                          continue;
                      else
                        continue;
                    else if(ptr[offset9] < c_b)
                      if(ptr[offset7] < c_b)
                        if(ptr[offset8] < c_b)
                          if(ptr[offset10] < c_b)
                            if(ptr[offset11] < c_b)
                              if(ptr[offset6] < c_b)
                                if(ptr[offset5] < c_b)
                                  if(ptr[offset4] < c_b)
                                    if(ptr[offset3] < c_b)
                                      {}
                                    else
                                      if(ptr[offset12] < c_b)
                                        {}
                                      else
                                        continue;
                                  else
                                    if(ptr[offset12] < c_b)
                                      if(ptr[offset13] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset12] < c_b)
                                    if(ptr[offset13] < c_b)
                                      if(ptr[offset14] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset12] < c_b)
                                  if(ptr[offset13] < c_b)
                                    if(ptr[offset14] < c_b)
                                      if(ptr[offset15] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          continue;
                      else
                        continue;
                    else
                      continue;
                else if(ptr[offset0] < c_b)
                  if(ptr[offset2] > cb)
                    if(ptr[offset9] > cb)
                      if(ptr[offset7] > cb)
                        if(ptr[offset8] > cb)
                          if(ptr[offset6] > cb)
                            if(ptr[offset5] > cb)
                              if(ptr[offset4] > cb)
                                if(ptr[offset3] > cb)
                                  if(ptr[offset1] > cb)
                                    {}
                                  else
                                    if(ptr[offset10] > cb)
                                      {}
                                    else
                                      continue;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      if(ptr[offset12] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    if(ptr[offset12] > cb)
                                      if(ptr[offset13] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset10] > cb)
                                if(ptr[offset11] > cb)
                                  if(ptr[offset12] > cb)
                                    if(ptr[offset13] > cb)
                                      if(ptr[offset14] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            if(ptr[offset10] > cb)
                              if(ptr[offset11] > cb)
                                if(ptr[offset12] > cb)
                                  if(ptr[offset13] > cb)
                                    if(ptr[offset14] > cb)
                                      if(ptr[offset15] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                        else
                          continue;
                      else
                        continue;
                    else if(ptr[offset9] < c_b)
                      if(ptr[offset10] < c_b)
                        if(ptr[offset11] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset12] < c_b)
                              if(ptr[offset13] < c_b)
                                if(ptr[offset14] < c_b)
                                  if(ptr[offset15] < c_b)
                                    {}
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset7] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset5] < c_b)
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset7] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset4] < c_b)
                                  if(ptr[offset5] < c_b)
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset7] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset3] < c_b)
                                if(ptr[offset4] < c_b)
                                  if(ptr[offset5] < c_b)
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset7] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            if(ptr[offset1] < c_b)
                              if(ptr[offset12] < c_b)
                                if(ptr[offset13] < c_b)
                                  if(ptr[offset14] < c_b)
                                    if(ptr[offset15] < c_b)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                        else
                          continue;
                      else
                        continue;
                    else
                      continue;
                  else if(ptr[offset2] < c_b)
                    if(ptr[offset4] > cb)
                      if(ptr[offset11] > cb)
                        if(ptr[offset7] > cb)
                          if(ptr[offset8] > cb)
                            if(ptr[offset9] > cb)
                              if(ptr[offset10] > cb)
                                if(ptr[offset6] > cb)
                                  if(ptr[offset5] > cb)
                                    if(ptr[offset3] > cb)
                                      {}
                                    else
                                      if(ptr[offset12] > cb)
                                        {}
                                      else
                                        continue;
                                  else
                                    if(ptr[offset12] > cb)
                                      if(ptr[offset13] > cb)
                                        if(ptr[offset14] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset12] > cb)
                                    if(ptr[offset13] > cb)
                                      if(ptr[offset14] > cb)
                                        if(ptr[offset15] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          continue;
                      else if(ptr[offset11] < c_b)
                        if(ptr[offset12] < c_b)
                          if(ptr[offset13] < c_b)
                            if(ptr[offset10] < c_b)
                              if(ptr[offset14] < c_b)
                                if(ptr[offset15] < c_b)
                                  if(ptr[offset1] < c_b)
                                    {}
                                  else
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset9] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        if(ptr[offset9] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset5] < c_b)
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        if(ptr[offset9] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset1] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset14] < c_b)
                                    if(ptr[offset15] < c_b)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            continue;
                        else
                          continue;
                      else
                        continue;
                    else if(ptr[offset4] < c_b)
                      if(ptr[offset5] > cb)
                        if(ptr[offset12] > cb)
                          if(ptr[offset7] > cb)
                            if(ptr[offset8] > cb)
                              if(ptr[offset9] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    if(ptr[offset13] > cb)
                                      if(ptr[offset6] > cb)
                                        {}
                                      else
                                        if(ptr[offset14] > cb)
                                          if(ptr[offset15] > cb)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                        else if(ptr[offset12] < c_b)
                          if(ptr[offset13] < c_b)
                            if(ptr[offset14] < c_b)
                              if(ptr[offset15] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset3] < c_b)
                                    {}
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset9] < c_b)
                                      if(ptr[offset10] < c_b)
                                        if(ptr[offset11] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset7] < c_b)
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset9] < c_b)
                                        if(ptr[offset10] < c_b)
                                          if(ptr[offset11] < c_b)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          continue;
                      else if(ptr[offset5] < c_b)
                        if(ptr[offset7] > cb)
                          if(ptr[offset14] > cb)
                            if(ptr[offset8] > cb)
                              if(ptr[offset9] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    if(ptr[offset12] > cb)
                                      if(ptr[offset13] > cb)
                                        if(ptr[offset6] > cb)
                                          {}
                                        else
                                          if(ptr[offset15] > cb)
                                            {}
                                          else
                                            continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                          else if(ptr[offset14] < c_b)
                            if(ptr[offset15] < c_b)
                              if(ptr[offset1] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset6] < c_b)
                                    {}
                                  else
                                    if(ptr[offset13] < c_b)
                                      {}
                                    else
                                      continue;
                                else
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      if(ptr[offset12] < c_b)
                                        if(ptr[offset13] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset9] < c_b)
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        if(ptr[offset12] < c_b)
                                          if(ptr[offset13] < c_b)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              continue;
                          else
                            continue;
                        else if(ptr[offset7] < c_b)
                          if(ptr[offset3] < c_b)
                            if(ptr[offset1] < c_b)
                              if(ptr[offset6] < c_b)
                                if(ptr[offset8] < c_b)
                                  {}
                                else
                                  if(ptr[offset15] < c_b)
                                    {}
                                  else
                                    continue;
                              else
                                if(ptr[offset13] < c_b)
                                  if(ptr[offset14] < c_b)
                                    if(ptr[offset15] < c_b)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset8] < c_b)
                                if(ptr[offset9] < c_b)
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset6] < c_b)
                                      {}
                                    else
                                      if(ptr[offset11] < c_b)
                                        if(ptr[offset12] < c_b)
                                          if(ptr[offset13] < c_b)
                                            if(ptr[offset14] < c_b)
                                              if(ptr[offset15] < c_b)
                                                {}
                                              else
                                                continue;
                                            else
                                              continue;
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            if(ptr[offset10] < c_b)
                              if(ptr[offset11] < c_b)
                                if(ptr[offset12] < c_b)
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset9] < c_b)
                                      if(ptr[offset6] < c_b)
                                        {}
                                      else
                                        if(ptr[offset13] < c_b)
                                          if(ptr[offset14] < c_b)
                                            if(ptr[offset15] < c_b)
                                              {}
                                            else
                                              continue;
                                          else
                                            continue;
                                        else
                                          continue;
                                    else
                                      if(ptr[offset1] < c_b)
                                        if(ptr[offset13] < c_b)
                                          if(ptr[offset14] < c_b)
                                            if(ptr[offset15] < c_b)
                                              {}
                                            else
                                              continue;
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                  else
                                    if(ptr[offset1] < c_b)
                                      if(ptr[offset13] < c_b)
                                        if(ptr[offset14] < c_b)
                                          if(ptr[offset15] < c_b)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                        else
                          if(ptr[offset14] < c_b)
                            if(ptr[offset15] < c_b)
                              if(ptr[offset1] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset6] < c_b)
                                    {}
                                  else
                                    if(ptr[offset13] < c_b)
                                      {}
                                    else
                                      continue;
                                else
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      if(ptr[offset12] < c_b)
                                        if(ptr[offset13] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset8] < c_b)
                                  if(ptr[offset9] < c_b)
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        if(ptr[offset12] < c_b)
                                          if(ptr[offset13] < c_b)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              continue;
                          else
                            continue;
                      else
                        if(ptr[offset12] > cb)
                          if(ptr[offset7] > cb)
                            if(ptr[offset8] > cb)
                              if(ptr[offset9] > cb)
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    if(ptr[offset13] > cb)
                                      if(ptr[offset14] > cb)
                                        if(ptr[offset6] > cb)
                                          {}
                                        else
                                          if(ptr[offset15] > cb)
                                            {}
                                          else
                                            continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                        else if(ptr[offset12] < c_b)
                          if(ptr[offset13] < c_b)
                            if(ptr[offset14] < c_b)
                              if(ptr[offset15] < c_b)
                                if(ptr[offset1] < c_b)
                                  if(ptr[offset3] < c_b)
                                    {}
                                  else
                                    if(ptr[offset10] < c_b)
                                      if(ptr[offset11] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset8] < c_b)
                                    if(ptr[offset9] < c_b)
                                      if(ptr[offset10] < c_b)
                                        if(ptr[offset11] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset6] < c_b)
                                  if(ptr[offset7] < c_b)
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset9] < c_b)
                                        if(ptr[offset10] < c_b)
                                          if(ptr[offset11] < c_b)
                                            {}
                                          else
                                            continue;
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          continue;
                    else
                      if(ptr[offset11] > cb)
                        if(ptr[offset7] > cb)
                          if(ptr[offset8] > cb)
                            if(ptr[offset9] > cb)
                              if(ptr[offset10] > cb)
                                if(ptr[offset12] > cb)
                                  if(ptr[offset13] > cb)
                                    if(ptr[offset6] > cb)
                                      if(ptr[offset5] > cb)
                                        {}
                                      else
                                        if(ptr[offset14] > cb)
                                          {}
                                        else
                                          continue;
                                    else
                                      if(ptr[offset14] > cb)
                                        if(ptr[offset15] > cb)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          continue;
                      else if(ptr[offset11] < c_b)
                        if(ptr[offset12] < c_b)
                          if(ptr[offset13] < c_b)
                            if(ptr[offset10] < c_b)
                              if(ptr[offset14] < c_b)
                                if(ptr[offset15] < c_b)
                                  if(ptr[offset1] < c_b)
                                    {}
                                  else
                                    if(ptr[offset8] < c_b)
                                      if(ptr[offset9] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        if(ptr[offset9] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset5] < c_b)
                                  if(ptr[offset6] < c_b)
                                    if(ptr[offset7] < c_b)
                                      if(ptr[offset8] < c_b)
                                        if(ptr[offset9] < c_b)
                                          {}
                                        else
                                          continue;
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset1] < c_b)
                                if(ptr[offset3] < c_b)
                                  if(ptr[offset14] < c_b)
                                    if(ptr[offset15] < c_b)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            continue;
                        else
                          continue;
                      else
                        continue;
                  else
                    if(ptr[offset9] > cb)
                      if(ptr[offset7] > cb)
                        if(ptr[offset8] > cb)
                          if(ptr[offset10] > cb)
                            if(ptr[offset11] > cb)
                              if(ptr[offset6] > cb)
                                if(ptr[offset5] > cb)
                                  if(ptr[offset4] > cb)
                                    if(ptr[offset3] > cb)
                                      {}
                                    else
                                      if(ptr[offset12] > cb)
                                        {}
                                      else
                                        continue;
                                  else
                                    if(ptr[offset12] > cb)
                                      if(ptr[offset13] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset12] > cb)
                                    if(ptr[offset13] > cb)
                                      if(ptr[offset14] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset12] > cb)
                                  if(ptr[offset13] > cb)
                                    if(ptr[offset14] > cb)
                                      if(ptr[offset15] > cb)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              continue;
                          else
                            continue;
                        else
                          continue;
                      else
                        continue;
                    else if(ptr[offset9] < c_b)
                      if(ptr[offset10] < c_b)
                        if(ptr[offset11] < c_b)
                          if(ptr[offset8] < c_b)
                            if(ptr[offset12] < c_b)
                              if(ptr[offset13] < c_b)
                                if(ptr[offset14] < c_b)
                                  if(ptr[offset15] < c_b)
                                    {}
                                  else
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset7] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                else
                                  if(ptr[offset5] < c_b)
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset7] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset4] < c_b)
                                  if(ptr[offset5] < c_b)
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset7] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset3] < c_b)
                                if(ptr[offset4] < c_b)
                                  if(ptr[offset5] < c_b)
                                    if(ptr[offset6] < c_b)
                                      if(ptr[offset7] < c_b)
                                        {}
                                      else
                                        continue;
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            if(ptr[offset1] < c_b)
                              if(ptr[offset12] < c_b)
                                if(ptr[offset13] < c_b)
                                  if(ptr[offset14] < c_b)
                                    if(ptr[offset15] < c_b)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                        else
                          continue;
                      else
                        continue;
                    else
                      continue;
                else
                  if(ptr[offset7] > cb)
                    if(ptr[offset8] > cb)
                      if(ptr[offset9] > cb)
                        if(ptr[offset6] > cb)
                          if(ptr[offset5] > cb)
                            if(ptr[offset4] > cb)
                              if(ptr[offset3] > cb)
                                if(ptr[offset2] > cb)
                                  if(ptr[offset1] > cb)
                                    {}
                                  else
                                    if(ptr[offset10] > cb)
                                      {}
                                    else
                                      continue;
                                else
                                  if(ptr[offset10] > cb)
                                    if(ptr[offset11] > cb)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset10] > cb)
                                  if(ptr[offset11] > cb)
                                    if(ptr[offset12] > cb)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset10] > cb)
                                if(ptr[offset11] > cb)
                                  if(ptr[offset12] > cb)
                                    if(ptr[offset13] > cb)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            if(ptr[offset10] > cb)
                              if(ptr[offset11] > cb)
                                if(ptr[offset12] > cb)
                                  if(ptr[offset13] > cb)
                                    if(ptr[offset14] > cb)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                        else
                          if(ptr[offset10] > cb)
                            if(ptr[offset11] > cb)
                              if(ptr[offset12] > cb)
                                if(ptr[offset13] > cb)
                                  if(ptr[offset14] > cb)
                                    if(ptr[offset15] > cb)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                      else
                        continue;
                    else
                      continue;
                  else if(ptr[offset7] < c_b)
                    if(ptr[offset8] < c_b)
                      if(ptr[offset9] < c_b)
                        if(ptr[offset6] < c_b)
                          if(ptr[offset5] < c_b)
                            if(ptr[offset4] < c_b)
                              if(ptr[offset3] < c_b)
                                if(ptr[offset2] < c_b)
                                  if(ptr[offset1] < c_b)
                                    {}
                                  else
                                    if(ptr[offset10] < c_b)
                                      {}
                                    else
                                      continue;
                                else
                                  if(ptr[offset10] < c_b)
                                    if(ptr[offset11] < c_b)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                              else
                                if(ptr[offset10] < c_b)
                                  if(ptr[offset11] < c_b)
                                    if(ptr[offset12] < c_b)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                            else
                              if(ptr[offset10] < c_b)
                                if(ptr[offset11] < c_b)
                                  if(ptr[offset12] < c_b)
                                    if(ptr[offset13] < c_b)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                          else
                            if(ptr[offset10] < c_b)
                              if(ptr[offset11] < c_b)
                                if(ptr[offset12] < c_b)
                                  if(ptr[offset13] < c_b)
                                    if(ptr[offset14] < c_b)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                        else
                          if(ptr[offset10] < c_b)
                            if(ptr[offset11] < c_b)
                              if(ptr[offset12] < c_b)
                                if(ptr[offset13] < c_b)
                                  if(ptr[offset14] < c_b)
                                    if(ptr[offset15] < c_b)
                                      {}
                                    else
                                      continue;
                                  else
                                    continue;
                                else
                                  continue;
                              else
                                continue;
                            else
                              continue;
                          else
                            continue;
                      else
                        continue;
                    else
                      continue;
                  else
                    continue;
            }
            if(total == nExpectedCorners)
            {
                if(nExpectedCorners == 0)
                {
                    nExpectedCorners = 512;
                    keypoints.reserve(nExpectedCorners);
                }
                else
                {
                    nExpectedCorners *= 2;
                    keypoints.reserve(nExpectedCorners);
                }
            }
            keypoints.push_back(KeyPoint(Point2f((float)x, (float)y), 1.0f));
            total++;
        }
    }
}


void AGAST(InputArray _img, std::vector<KeyPoint>& keypoints, int threshold, bool nonmax_suppression)
{
    AGAST(_img, keypoints, threshold, nonmax_suppression, AgastFeatureDetector::OAST_9_16);
}


class AgastFeatureDetector_Impl : public AgastFeatureDetector
{
public:
    AgastFeatureDetector_Impl( int _threshold, bool _nonmaxSuppression, int _type )
    : threshold(_threshold), nonmaxSuppression(_nonmaxSuppression), type((short)_type)
    {}

    void detect( InputArray _image, std::vector<KeyPoint>& keypoints, InputArray _mask )
    {
        Mat mask = _mask.getMat(), grayImage;
        UMat ugrayImage;
        _InputArray gray = _image;
        if( _image.type() != CV_8U )
        {
            _OutputArray ogray = _image.isUMat() ? _OutputArray(ugrayImage) : _OutputArray(grayImage);
            cvtColor( _image, ogray, COLOR_BGR2GRAY );
            gray = ogray;
        }
        AGAST( gray, keypoints, threshold, nonmaxSuppression, type );
        KeyPointsFilter::runByPixelsMask( keypoints, mask );
    }

    void set(int prop, double value)
    {
        if(prop == THRESHOLD)
            threshold = cvRound(value);
        else if(prop == NONMAX_SUPPRESSION)
            nonmaxSuppression = value != 0;
        else
            CV_Error(Error::StsBadArg, "");
    }

    double get(int prop) const
    {
        if(prop == THRESHOLD)
            return threshold;
        if(prop == NONMAX_SUPPRESSION)
            return nonmaxSuppression;
        CV_Error(Error::StsBadArg, "");
        return 0;
    }

    void setThreshold(int threshold_) { threshold = threshold_; }
    int getThreshold() const { return threshold; }

    void setNonmaxSuppression(bool f) { nonmaxSuppression = f; }
    bool getNonmaxSuppression() const { return nonmaxSuppression; }

    void setType(int type_) { type = type_; }
    int getType() const { return type; }

    int threshold;
    bool nonmaxSuppression;
    int type;
};

Ptr<AgastFeatureDetector> AgastFeatureDetector::create( int threshold, bool nonmaxSuppression, int type )
{
    return makePtr<AgastFeatureDetector_Impl>(threshold, nonmaxSuppression, type);
}

void AGAST(InputArray _img, std::vector<KeyPoint>& keypoints, int threshold, bool nonmax_suppression, int type)
{
    // detect
    switch(type) {
      case AgastFeatureDetector::AGAST_5_8:
        AGAST_5_8(_img, keypoints, threshold);
        break;
      case AgastFeatureDetector::AGAST_7_12d:
        AGAST_7_12d(_img, keypoints, threshold);
        break;
      case AgastFeatureDetector::AGAST_7_12s:
        AGAST_7_12s(_img, keypoints, threshold);
        break;
      case AgastFeatureDetector::OAST_9_16:
        OAST_9_16(_img, keypoints, threshold);
        break;
    }

    cv::Mat img = _img.getMat();

    // score
    int pixel_[16];
    makeAgastOffsets(pixel_, (int)img.step, type);

    std::vector<KeyPoint>::iterator kpt;
    for(kpt = keypoints.begin(); kpt != keypoints.end(); kpt++)
    {
        switch(type) {
          case AgastFeatureDetector::AGAST_5_8:
            kpt->response = (float)agast_cornerScore<AgastFeatureDetector::AGAST_5_8>
                (&img.at<uchar>((int)kpt->pt.y, (int)kpt->pt.x), pixel_, threshold);
            break;
          case AgastFeatureDetector::AGAST_7_12d:
            kpt->response = (float)agast_cornerScore<AgastFeatureDetector::AGAST_7_12d>
                (&img.at<uchar>((int)kpt->pt.y, (int)kpt->pt.x), pixel_, threshold);
            break;
          case AgastFeatureDetector::AGAST_7_12s:
            kpt->response = (float)agast_cornerScore<AgastFeatureDetector::AGAST_7_12s>
                (&img.at<uchar>((int)kpt->pt.y, (int)kpt->pt.x), pixel_, threshold);
            break;
          case AgastFeatureDetector::OAST_9_16:
            kpt->response = (float)agast_cornerScore<AgastFeatureDetector::OAST_9_16>
                (&img.at<uchar>((int)kpt->pt.y, (int)kpt->pt.x), pixel_, threshold);
            break;
        }
    }
    // suppression
    if(nonmax_suppression)
    {
        size_t j;
        size_t curr_idx;
        size_t lastRow = 0, next_lastRow = 0;
        size_t num_Corners = keypoints.size();
        size_t lastRowCorner_ind = 0, next_lastRowCorner_ind = 0;

        std::vector<int> nmsFlags;
        std::vector<KeyPoint>::iterator currCorner_nms;
        std::vector<KeyPoint>::const_iterator currCorner;

        currCorner = keypoints.begin();

        nmsFlags.resize((int)num_Corners);

        // set all flags to MAXIMUM
        for(j = 0; j < num_Corners; j++)
            nmsFlags[j] = -1;

        for(curr_idx = 0; curr_idx < num_Corners; curr_idx++)
        {
            int t;
            // check above
            if(lastRow + 1 < currCorner->pt.y)
            {
                lastRow = next_lastRow;
                lastRowCorner_ind = next_lastRowCorner_ind;
            }
            if(next_lastRow != currCorner->pt.y)
            {
                next_lastRow = (size_t) currCorner->pt.y;
                next_lastRowCorner_ind = curr_idx;
            }
            if(lastRow + 1 == currCorner->pt.y)
            {
                // find the corner above the current one
                while( (keypoints[lastRowCorner_ind].pt.x < currCorner->pt.x)
                    && (keypoints[lastRowCorner_ind].pt.y == lastRow) )
                    lastRowCorner_ind++;

                    if( (keypoints[lastRowCorner_ind].pt.x == currCorner->pt.x)
                     && (lastRowCorner_ind != curr_idx) )
                    {
                        size_t w = lastRowCorner_ind;
                        // find the maximum in this block
                        while(nmsFlags[w] != -1)
                            w = nmsFlags[w];

                        if(keypoints[curr_idx].response < keypoints[w].response)
                            nmsFlags[curr_idx] = (int)w;
                        else
                            nmsFlags[w] = (int)curr_idx;
                    }
            }

            // check left
            t = (int)curr_idx - 1;
            if( (curr_idx != 0) && (keypoints[t].pt.y == currCorner->pt.y)
             && (keypoints[t].pt.x + 1 == currCorner->pt.x) )
            {
                int currCornerMaxAbove_ind = nmsFlags[curr_idx];
                // find the maximum in that area
                while(nmsFlags[t] != -1)
                    t = nmsFlags[t];
                // no maximum above
                if(currCornerMaxAbove_ind == -1)
                {
                    if((size_t)t != curr_idx)
                    {
                        if ( keypoints[curr_idx].response < keypoints[t].response )
                            nmsFlags[curr_idx] = t;
                        else
                            nmsFlags[t] = (int)curr_idx;
                    }
                }
                else // maximum above
                {
                    if(t != currCornerMaxAbove_ind)
                    {
                        if(keypoints[currCornerMaxAbove_ind].response < keypoints[t].response)
                        {
                            nmsFlags[currCornerMaxAbove_ind] = t;
                            nmsFlags[curr_idx] = t;
                        }
                        else
                        {
                            nmsFlags[t] = currCornerMaxAbove_ind;
                            nmsFlags[curr_idx] = currCornerMaxAbove_ind;
                        }
                    }
                }
            }
            currCorner++;
        }

        // marks non-maximum corners
        for(curr_idx = 0; curr_idx < num_Corners; curr_idx++)
        {
            if (nmsFlags[curr_idx] != -1)
                keypoints[curr_idx].response = -1;
        }

        // erase non-maximum corners
        for (j = keypoints.size(); j > 0; j--)
        {
            if (keypoints[j - 1].response == -1)
                keypoints.erase(keypoints.begin() + j - 1 );
        }
    }
}

} // END NAMESPACE CV
