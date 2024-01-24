import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import web_api_model

Window
{
    width: 640
    height: 480
    visible: true
    title: qsTr("Web API")

    function onWebApiChaged(content, ahint)
    {
        api_descr.text = content;
        hint_field.text = ahint
    }

    AWeb_api_model
    {
        id: web_model

    }

    RowLayout
    {
        anchors.fill: parent
        ListView
        {
            id: web_model_view
            model: web_model
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            focus: true
            delegate: Item
            {
                width: web_model_view.width
                height: 20
                Column
                {
                    padding: 5
                    Text { text: modelData}
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        web_model_view.currentIndex = index
                    }
                }
            }
            Layout.fillHeight: true
            Layout.preferredWidth: 240
            onCurrentIndexChanged: web_model.indexChanged(currentIndex)
        }

        ColumnLayout
        {
            TextEdit
            {
                id: api_descr

                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            TextField
            {
                id: hint_field
                Layout.preferredHeight: 40
                Layout.fillWidth: true
            }

            Button
            {
                id: apply_hint
                text: "Применить"
                Layout.fillWidth: true
                onClicked: web_model.apply_hint(hint_field.text)
            }
        }
    }
    Component.onCompleted: {
        web_model.curr_api_changed.connect(onWebApiChaged)
    }
}
