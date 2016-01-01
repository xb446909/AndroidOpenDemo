import QtQuick 2.0
import QtMultimedia 5.5

Item {
    id: cameraUI

    Camera {
        id: camera
        captureMode: Camera.CaptureStillImage

    }

    VideoOutput {
        id: viewfinder
        visible: true

        x: 0
        y: 0
        width: parent.width
        height: parent.height

        source: camera
    }
}

