import QtQuick 2.0
import QtMultimedia 5.5

Item {
    id: cameraUI

    signal imageCaptured(string preview)

    Camera {
        id: camera
        objectName: "cameraObj"
        captureMode: Camera.CaptureStillImage

        imageCapture {
            objectName: "imageCaptureObj"
            onImageCaptured: {
                imageCaptured(preview)
                console.log("qml: Captured");
            }
        }
    }

    VideoOutput {
        id: viewfinder
        objectName: "videoOutputObj"
        visible: true
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        source: camera

        MouseArea {
                    anchors.fill: parent;
                    onClicked: camera.imageCapture.capture();
                }

/*
        Repeater {
                  model: camera.focus.focusZones

                  Rectangle {
                      border {
                          width: 2
                          color: status == Camera.FocusAreaFocused ? "green" : "white"
                      }
                      color: "transparent"

                      // Map from the relative, normalized frame coordinates
                      property variant mappedRect: viewfinder.mapNormalizedRectToItem(area);

                      x: mappedRect.x
                      y: mappedRect.y
                      width: mappedRect.width
                      height: mappedRect.height
                  }
            }*/
    }
}

