import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

ApplicationWindow {
    visible: true
    width: 800
    height: 480
    visibility: "FullScreen"
    title: qsTr("SmartHome v1.0")

    /*menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }*/

    ColumnLayout
    {

        ToolButton
        {
            Layout.margins: 20,20,20,20
            iconSource: "/settings.png"
            onClicked:
            {
                var component = Qt.createComponent("settings.qml")
                var settingsWindow = component.createObject("settingsWindow")
                settingsWindow.show()
            }
        }

        ToolButton
        {
            Layout.margins: 20,20,20,20
            iconSource: "/close.png"
            onClicked: closeDialog.open();
        }
    }

    MessageDialog
    {
        id: unimplementedDialog
        text: "This function is yet to be implemented"
    }

    MessageDialog
    {
        id: closeDialog
        text: "Do you really want to close the application?"
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            Qt.quit()
        }
    }









}
