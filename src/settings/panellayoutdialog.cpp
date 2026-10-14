/*
 * SPDX-FileCopyrightText: 2026 The ghostwriter contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "appsettings.h"
#include "panellayoutdialog.h"

namespace ghostwriter
{
class PanelLayoutDialogPrivate
{
public:
    AppSettings *appSettings = nullptr;
    QComboBox *sidebarComboBox = nullptr;
    QComboBox *editorComboBox = nullptr;
    QComboBox *previewComboBox = nullptr;
    bool updatingCombos = false;

    void populateCombo(QComboBox *combo) const;
    void refreshCombos();
    void onPositionChosen(WindowPanel panel, int comboIndex);
};

void PanelLayoutDialogPrivate::populateCombo(QComboBox *combo) const
{
    combo->addItem(PanelLayoutDialog::tr("Left"), static_cast<int>(LeftPanelSlot));
    combo->addItem(PanelLayoutDialog::tr("Center"), static_cast<int>(CenterPanelSlot));
    combo->addItem(PanelLayoutDialog::tr("Right"), static_cast<int>(RightPanelSlot));
}

void PanelLayoutDialogPrivate::refreshCombos()
{
    updatingCombos = true;

    const PanelLayout layout = appSettings->panelLayout();
    sidebarComboBox->setCurrentIndex(static_cast<int>(layout.slotOf(SidebarPanel)));
    editorComboBox->setCurrentIndex(static_cast<int>(layout.slotOf(EditorPanel)));
    previewComboBox->setCurrentIndex(static_cast<int>(layout.slotOf(HtmlPreviewPanel)));

    updatingCombos = false;
}

void PanelLayoutDialogPrivate::onPositionChosen(WindowPanel panel, int comboIndex)
{
    if (updatingCombos || (comboIndex < 0)) {
        return;
    }

    PanelLayout layout = appSettings->panelLayout();
    layout.setPanelSlot(panel, static_cast<PanelSlot>(comboIndex));

    if (!layout.isValid()) {
        refreshCombos();
        return;
    }

    appSettings->setPanelLayout(layout);
    refreshCombos();
}

PanelLayoutDialog::PanelLayoutDialog(QWidget *parent)
    : QDialog(parent)
    , d_ptr(new PanelLayoutDialogPrivate())
{
    Q_D(PanelLayoutDialog);

    setWindowTitle(tr("Configure Panels"));
    d->appSettings = AppSettings::instance();

    auto *layout = new QVBoxLayout(this);
    auto *form = new QFormLayout();
    layout->addLayout(form);

    auto *description = new QLabel(
        tr("Choose where each panel appears. Selecting a position that is "
           "already used will swap the two panels."),
        this);
    description->setWordWrap(true);
    layout->insertWidget(0, description);

    d->sidebarComboBox = new QComboBox(this);
    d->editorComboBox = new QComboBox(this);
    d->previewComboBox = new QComboBox(this);
    d->populateCombo(d->sidebarComboBox);
    d->populateCombo(d->editorComboBox);
    d->populateCombo(d->previewComboBox);

    form->addRow(tr("Sidebar:"), d->sidebarComboBox);
    form->addRow(tr("Editor:"), d->editorComboBox);
    form->addRow(tr("Live Preview:"), d->previewComboBox);

    connect(d->sidebarComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [d](int index) {
                d->onPositionChosen(SidebarPanel, index);
            });
    connect(d->editorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [d](int index) {
                d->onPositionChosen(EditorPanel, index);
            });
    connect(d->previewComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [d](int index) {
                d->onPositionChosen(HtmlPreviewPanel, index);
            });

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    layout->addWidget(buttonBox);
    connect(buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked,
            this, &QDialog::close);

    d->refreshCombos();
}

PanelLayoutDialog::~PanelLayoutDialog()
{
}
} // namespace ghostwriter
