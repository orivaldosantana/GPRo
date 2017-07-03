/********************************************************************************
** Form generated from reading UI file 'SDFDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SDFDIALOG_H
#define UI_SDFDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SDFDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *lblFilename;
    QLabel *txtFilename;
    QLabel *lblContent;
    QLabel *txtContent;
    QCheckBox *qqIgnoreMissingValues;
    QCheckBox *qqCollisionLinksHidden;
    QCheckBox *qqJointsHidden;
    QCheckBox *qqConvexDecompose;
    QCheckBox *qqConvexDecomposeDlg;
    QCheckBox *qqCreateVisualLinks;
    QCheckBox *qqCenterModel;
    QCheckBox *qqModelDefinition;
    QCheckBox *qqAlternateMasks;
    QCheckBox *qqPositionCtrl;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *qqImport;
    QLabel *label;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *SDFDialog)
    {
        if (SDFDialog->objectName().isEmpty())
            SDFDialog->setObjectName(QStringLiteral("SDFDialog"));
        SDFDialog->setWindowModality(Qt::ApplicationModal);
        SDFDialog->resize(382, 459);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SDFDialog->sizePolicy().hasHeightForWidth());
        SDFDialog->setSizePolicy(sizePolicy);
        SDFDialog->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        SDFDialog->setModal(true);
        gridLayout = new QGridLayout(SDFDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        lblFilename = new QLabel(SDFDialog);
        lblFilename->setObjectName(QStringLiteral("lblFilename"));

        formLayout->setWidget(0, QFormLayout::LabelRole, lblFilename);

        txtFilename = new QLabel(SDFDialog);
        txtFilename->setObjectName(QStringLiteral("txtFilename"));
        txtFilename->setStyleSheet(QStringLiteral("font-weight: bold;"));

        formLayout->setWidget(0, QFormLayout::FieldRole, txtFilename);

        lblContent = new QLabel(SDFDialog);
        lblContent->setObjectName(QStringLiteral("lblContent"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lblContent);

        txtContent = new QLabel(SDFDialog);
        txtContent->setObjectName(QStringLiteral("txtContent"));
        txtContent->setWordWrap(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, txtContent);


        verticalLayout->addLayout(formLayout);

        qqIgnoreMissingValues = new QCheckBox(SDFDialog);
        qqIgnoreMissingValues->setObjectName(QStringLiteral("qqIgnoreMissingValues"));

        verticalLayout->addWidget(qqIgnoreMissingValues);

        qqCollisionLinksHidden = new QCheckBox(SDFDialog);
        qqCollisionLinksHidden->setObjectName(QStringLiteral("qqCollisionLinksHidden"));

        verticalLayout->addWidget(qqCollisionLinksHidden);

        qqJointsHidden = new QCheckBox(SDFDialog);
        qqJointsHidden->setObjectName(QStringLiteral("qqJointsHidden"));

        verticalLayout->addWidget(qqJointsHidden);

        qqConvexDecompose = new QCheckBox(SDFDialog);
        qqConvexDecompose->setObjectName(QStringLiteral("qqConvexDecompose"));

        verticalLayout->addWidget(qqConvexDecompose);

        qqConvexDecomposeDlg = new QCheckBox(SDFDialog);
        qqConvexDecomposeDlg->setObjectName(QStringLiteral("qqConvexDecomposeDlg"));

        verticalLayout->addWidget(qqConvexDecomposeDlg);

        qqCreateVisualLinks = new QCheckBox(SDFDialog);
        qqCreateVisualLinks->setObjectName(QStringLiteral("qqCreateVisualLinks"));

        verticalLayout->addWidget(qqCreateVisualLinks);

        qqCenterModel = new QCheckBox(SDFDialog);
        qqCenterModel->setObjectName(QStringLiteral("qqCenterModel"));

        verticalLayout->addWidget(qqCenterModel);

        qqModelDefinition = new QCheckBox(SDFDialog);
        qqModelDefinition->setObjectName(QStringLiteral("qqModelDefinition"));

        verticalLayout->addWidget(qqModelDefinition);

        qqAlternateMasks = new QCheckBox(SDFDialog);
        qqAlternateMasks->setObjectName(QStringLiteral("qqAlternateMasks"));

        verticalLayout->addWidget(qqAlternateMasks);

        qqPositionCtrl = new QCheckBox(SDFDialog);
        qqPositionCtrl->setObjectName(QStringLiteral("qqPositionCtrl"));

        verticalLayout->addWidget(qqPositionCtrl);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        qqImport = new QPushButton(SDFDialog);
        qqImport->setObjectName(QStringLiteral("qqImport"));

        horizontalLayout->addWidget(qqImport);


        verticalLayout->addLayout(horizontalLayout);

        label = new QLabel(SDFDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        QWidget::setTabOrder(qqCollisionLinksHidden, qqJointsHidden);
        QWidget::setTabOrder(qqJointsHidden, qqConvexDecomposeDlg);
        QWidget::setTabOrder(qqConvexDecomposeDlg, qqCreateVisualLinks);
        QWidget::setTabOrder(qqCreateVisualLinks, qqCenterModel);
        QWidget::setTabOrder(qqCenterModel, qqModelDefinition);
        QWidget::setTabOrder(qqModelDefinition, qqAlternateMasks);
        QWidget::setTabOrder(qqAlternateMasks, qqImport);

        retranslateUi(SDFDialog);

        QMetaObject::connectSlotsByName(SDFDialog);
    } // setupUi

    void retranslateUi(QDialog *SDFDialog)
    {
        SDFDialog->setWindowTitle(QApplication::translate("SDFDialog", "SDF Import", 0));
        lblFilename->setText(QApplication::translate("SDFDialog", "Filename:", 0));
        txtFilename->setText(QApplication::translate("SDFDialog", "N/A", 0));
        lblContent->setText(QApplication::translate("SDFDialog", "Content:", 0));
        txtContent->setText(QApplication::translate("SDFDialog", "N/A", 0));
        qqIgnoreMissingValues->setText(QApplication::translate("SDFDialog", "Ignore missing required values (SDF Parser)", 0));
        qqCollisionLinksHidden->setText(QApplication::translate("SDFDialog", "Assign collision links to layer 9 ", 0));
        qqJointsHidden->setText(QApplication::translate("SDFDialog", "Assign joints to layer 10", 0));
        qqConvexDecompose->setText(QApplication::translate("SDFDialog", "Convex decompose non-convex collision links", 0));
        qqConvexDecomposeDlg->setText(QApplication::translate("SDFDialog", "Show convex decomposition dialog", 0));
        qqCreateVisualLinks->setText(QApplication::translate("SDFDialog", "Create visual links if none", 0));
        qqCenterModel->setText(QApplication::translate("SDFDialog", "Center model above ground", 0));
        qqModelDefinition->setText(QApplication::translate("SDFDialog", "Prepare model definition if feasable", 0));
        qqAlternateMasks->setText(QApplication::translate("SDFDialog", "Alternate local respondable masks", 0));
        qqPositionCtrl->setText(QApplication::translate("SDFDialog", "Enable position control for revolute and prismatic joints", 0));
        qqImport->setText(QApplication::translate("SDFDialog", "Import", 0));
        label->setText(QApplication::translate("SDFDialog", "If you experience a sudden crash during the import operation, make sure to enable 'Show convex decomposition dialog' and adjust the parameters there. Another option would be to simply disable 'Convex decompose non-convex collision links'.", 0));
    } // retranslateUi

};

namespace Ui {
    class SDFDialog: public Ui_SDFDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SDFDIALOG_H
