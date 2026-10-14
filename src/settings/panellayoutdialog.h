/*
 * SPDX-FileCopyrightText: 2026 The ghostwriter contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PANELLAYOUTDIALOG_H
#define PANELLAYOUTDIALOG_H

#include <QDialog>
#include <QScopedPointer>

namespace ghostwriter
{
class PanelLayoutDialogPrivate;

/**
 * Lets the user assign each window panel (sidebar, editor, live preview)
 * to the left, center, or right splitter slot.
 */
class PanelLayoutDialog : public QDialog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PanelLayoutDialog)

public:
    explicit PanelLayoutDialog(QWidget *parent = nullptr);
    ~PanelLayoutDialog() override;

private:
    QScopedPointer<PanelLayoutDialogPrivate> d_ptr;
};
} // namespace ghostwriter

#endif // PANELLAYOUTDIALOG_H
