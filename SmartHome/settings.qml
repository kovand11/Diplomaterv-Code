import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

ApplicationWindow{
    width: 600
    height: 400
    id: settingsWindow
    title: qsTr("Settings")
//    visibility: "FullScreen"

    ColumnLayout
    {

        RowLayout
        {
            Layout.margins: 10,10,10,10
            Label{
                text: "Database server: "
            }
            TextField{
            }
            Label{
                text: "Database name: "
            }
            TextField{
            }
        }

        RowLayout
        {
            Layout.margins: 10,10,10,10
            Label{
                text: "User: "
            }
            TextField{
            }
            Label{
                text: "Password: "
            }
            TextField{
            }

        }

        RowLayout
        {
            Layout.margins: 10,10,10,10
            CheckBox{
                text: "Debug Widget"
            }
            CheckBox{
                text: "OpenDetector Widget"
            }
        }

        Label
        {
            Layout.margins: 10,10,10,10
            text: "Devices"
        }

        RowLayout{
            Layout.margins: 10,10,10,10


            TextArea
            {

            }

            Button
            {
                text: "keyboard"
            }
        }




    }

    RowLayout
    {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        Button{
            Layout.margins: 5,5,5,5
            text: "Apply"
            //signal to update the settings
        }
        Button{
            Layout.margins: 5,5,5,5
            text: "Close"
            onClicked: settingsWindow.close()
        }
    }

}
