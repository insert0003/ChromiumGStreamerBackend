// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ShapeDetector_h
#define ShapeDetector_h

#include "bindings/core/v8/ScriptPromise.h"
#include "bindings/core/v8/ScriptPromiseResolver.h"
#include "modules/ModulesExport.h"
#include "modules/canvas2d/CanvasRenderingContext2D.h"
#include "modules/shapedetection/FaceDetectorOptions.h"
#include "public/platform/modules/shapedetection/barcodedetection.mojom-blink.h"
#include "public/platform/modules/shapedetection/facedetection.mojom-blink.h"

namespace blink {

class LocalFrame;

class MODULES_EXPORT ShapeDetector
    : public GarbageCollectedFinalized<ShapeDetector> {
 public:
  enum class DetectorType {
    Face,
    Barcode
    // TODO(mcasas): Implement TextDetector after
    // https://github.com/WICG/shape-detection-api/issues/6
  };
  explicit ShapeDetector(LocalFrame&);
  ShapeDetector(LocalFrame&, const FaceDetectorOptions&);
  virtual ~ShapeDetector() = default;

  ScriptPromise detectShapes(ScriptState*,
                             DetectorType,
                             const CanvasImageSourceUnion&);
  DECLARE_VIRTUAL_TRACE();

 private:
  ScriptPromise detectShapesOnImageElement(DetectorType,
                                           ScriptPromiseResolver*,
                                           const HTMLImageElement*);
  ScriptPromise detectShapesOnImageBitmap(DetectorType,
                                          ScriptPromiseResolver*,
                                          ImageBitmap*);
  ScriptPromise detectShapesOnVideoElement(DetectorType,
                                           ScriptPromiseResolver*,
                                           const HTMLVideoElement*);

  ScriptPromise detectShapesOnData(DetectorType,
                                   ScriptPromiseResolver*,
                                   uint8_t* data,
                                   int size,
                                   int width,
                                   int height);
  void onDetectFaces(ScriptPromiseResolver*,
                     mojom::blink::FaceDetectionResultPtr);
  void onDetectBarcodes(ScriptPromiseResolver*,
                        Vector<mojom::blink::BarcodeDetectionResultPtr>);

  mojom::blink::FaceDetectionPtr m_faceService;
  mojom::blink::BarcodeDetectionPtr m_barcodeService;

  HeapHashSet<Member<ScriptPromiseResolver>> m_serviceRequests;

 protected:
  mojom::blink::FaceDetectorOptionsPtr m_faceDetectorOptions;
};

}  // namespace blink

#endif  // ShapeDetector_h
