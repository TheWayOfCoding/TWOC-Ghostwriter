/*
 * SPDX-FileCopyrightText: 2026 The ghostwriter contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef PANELLAYOUT_H
#define PANELLAYOUT_H

#include <QString>
#include <QStringList>

namespace ghostwriter
{
enum WindowPanel {
    SidebarPanel,
    EditorPanel,
    HtmlPreviewPanel
};

enum PanelSlot {
    LeftPanelSlot,
    CenterPanelSlot,
    RightPanelSlot
};

/**
 * Assignment of the three window panels to left, center, and right splitter
 * slots. Default matches the historical layout: sidebar, editor, preview.
 */
class PanelLayout
{
public:
    WindowPanel left = SidebarPanel;
    WindowPanel center = EditorPanel;
    WindowPanel right = HtmlPreviewPanel;

    static PanelLayout defaults()
    {
        return PanelLayout();
    }

    static QString name(WindowPanel panel)
    {
        switch (panel) {
        case EditorPanel:
            return QStringLiteral("editor");
        case HtmlPreviewPanel:
            return QStringLiteral("preview");
        case SidebarPanel:
        default:
            return QStringLiteral("sidebar");
        }
    }

    static WindowPanel fromName(const QString &value)
    {
        if (value == QLatin1String("editor")) {
            return EditorPanel;
        }

        if (value == QLatin1String("preview")) {
            return HtmlPreviewPanel;
        }

        return SidebarPanel;
    }

    static PanelLayout fromNames(const QStringList &names)
    {
        if (names.size() != 3) {
            return defaults();
        }

        PanelLayout layout;
        layout.left = fromName(names.at(0));
        layout.center = fromName(names.at(1));
        layout.right = fromName(names.at(2));

        if (!layout.isValid()) {
            return defaults();
        }

        return layout;
    }

    QStringList toNames() const
    {
        return QStringList{name(left), name(center), name(right)};
    }

    bool isValid() const
    {
        return (left != center)
            && (left != right)
            && (center != right)
            && contains(SidebarPanel)
            && contains(EditorPanel)
            && contains(HtmlPreviewPanel);
    }

    bool contains(WindowPanel panel) const
    {
        return (left == panel) || (center == panel) || (right == panel);
    }

    WindowPanel panelAt(PanelSlot slot) const
    {
        switch (slot) {
        case CenterPanelSlot:
            return center;
        case RightPanelSlot:
            return right;
        case LeftPanelSlot:
        default:
            return left;
        }
    }

    PanelSlot slotOf(WindowPanel panel) const
    {
        if (center == panel) {
            return CenterPanelSlot;
        }

        if (right == panel) {
            return RightPanelSlot;
        }

        return LeftPanelSlot;
    }

    void setAt(PanelSlot slot, WindowPanel panel)
    {
        switch (slot) {
        case CenterPanelSlot:
            center = panel;
            break;
        case RightPanelSlot:
            right = panel;
            break;
        case LeftPanelSlot:
        default:
            left = panel;
            break;
        }
    }

    /**
     * Moves panel into slot, swapping with whatever currently occupies it.
     */
    void setPanelSlot(WindowPanel panel, PanelSlot slot)
    {
        const PanelSlot currentSlot = slotOf(panel);

        if (currentSlot == slot) {
            return;
        }

        const WindowPanel occupant = panelAt(slot);
        setAt(slot, panel);
        setAt(currentSlot, occupant);
    }

    bool operator==(const PanelLayout &other) const
    {
        return (left == other.left)
            && (center == other.center)
            && (right == other.right);
    }

    bool operator!=(const PanelLayout &other) const
    {
        return !(*this == other);
    }
};
} // namespace ghostwriter

#endif // PANELLAYOUT_H
