/*
 * Copyright 2005 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Sun designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Sun in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 */

#include "jni.h"
#include "jni_util.h"
#include "jlong.h"
#include <string.h>

#include "sun_java2d_pipe_RenderBuffer.h"

/**
 * Note: The code in this file is nearly identical to that in
 *       java/nio/Bits.c...
 */

#define MBYTE 1048576

JNIEXPORT void JNICALL
Java_sun_java2d_pipe_RenderBuffer_copyFromArray
    (JNIEnv *env, jclass rb,
     jobject srcArray, jlong srcPos, jlong dstAddr, jlong length)
{
    jbyte *bytes;
    size_t size;

    while (length > 0) {
        /*
         * Copy no more than one megabyte at a time, to allow for GC.
         * (Probably not an issue for STR, since our buffer size is likely
         * much smaller than a megabyte, but just in case...)
         */
        size = (size_t)(length > MBYTE ? MBYTE : length);

        bytes = (*env)->GetPrimitiveArrayCritical(env, srcArray, NULL);
        if (bytes == NULL) {
            JNU_ThrowInternalError(env, "Unable to get array");
            return;
        }

        memcpy(jlong_to_ptr(dstAddr), bytes + srcPos, size);

        (*env)->ReleasePrimitiveArrayCritical(env, srcArray,
                                              bytes, JNI_ABORT);

        length -= size;
        dstAddr += size;
        srcPos += size;
    }
}
