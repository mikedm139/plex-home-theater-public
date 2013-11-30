//
//  GUIWindowMediaFilterView.h
//  Plex
//
//  Created by Tobias Hieta <tobias@plexapp.com> on 2012-11-19.
//  Copyright 2012 Plex Inc. All rights reserved.
//

#ifndef GUIWINDOWMEDIAFILTERVIEW_H
#define GUIWINDOWMEDIAFILTERVIEW_H

#include "video/windows/GUIWindowVideoNav.h"
#include "guilib/GUIButtonControl.h"
#include "guilib/GUIRadioButtonControl.h"
#include "StringUtils.h"
#include "JobManager.h"
#include "PlexContentPlayerMixin.h"
#include "threads/Event.h"
#include "Filters/PlexSectionFilter.h"
#include "guilib/GUIButtonControl.h"
#include "PlexNavigationHelper.h"

#define FILTER_PRIMARY_CONTAINER     19000
#define FILTER_SECONDARY_CONTAINER   19001
#define FILTER_BUTTON                19005
#define FILTER_RADIO_BUTTON          19006
#define FILTER_ACTIVE_LABEL          19007

#define SORT_LIST                    19010
#define SORT_ORDER_BUTTON            19011

#define FILTER_PRIMARY_BUTTONS_START 30000
#define FILTER_SECONDARY_BUTTONS_START 31000
#define SORT_BUTTONS_START           32000

#define FILTER_BUTTONS_START FILTER_PRIMARY_BUTTONS_START
#define FILTER_BUTTONS_STOP SORT_BUTTONS_START + 100
#define FILTER_CLEAR_FILTER_BUTTON FILTER_BUTTONS_START - 1

#define FILTER_PRIMARY_LABEL         19008
#define FILTER_SECONDARY_LABEL       10009
#define SORT_LABEL                   19019


class CGUIPlexMediaWindow : public CGUIMediaWindow, public IJobCallback, public PlexContentPlayerMixin
{    
  public:
    CGUIPlexMediaWindow(int windowId = WINDOW_VIDEO_NAV, const CStdString &xml = "MyVideoNav.xml") :
      CGUIMediaWindow(windowId, xml), m_returningFromSkinLoad(false), m_pagingOffset(0), m_currentJobId(-1), m_hasAdvancedFilters(false), m_clearFilterButton(NULL) {};
    bool OnMessage(CGUIMessage &message);
    bool OnAction(const CAction& action);
    virtual bool GetDirectory(const CStdString &strDirectory, CFileItemList &items);
    void GetContextButtons(int itemNumber, CContextButtons &buttons);

    bool Update(const CStdString &strDirectory, bool updateFilterPath, bool updateFromFilter);
    bool Update(const CStdString &strDirectory, bool updateFilterPath);
    bool OnSelect(int item);
    bool OnPlayMedia(int iItem);
    bool OnContextButton(int itemNumber, CONTEXT_BUTTON button);

    void ShuffleItem(CFileItemPtr item);
    void QueueItem(CFileItemPtr item);
    void QueueItems(const CFileItemList &list, CFileItemPtr startItem=CFileItemPtr());

    bool OnBack(int actionID);
    void OnFilterButton(int filterButtonId);
    void OnFilterSelected(const std::string& filterKey, int filterButtonId);
    static CURL GetRealDirectoryUrl(const CStdString &strDirectory);

    void CheckPlexFilters(CFileItemList &list);
    
    void OnQueueItem(int iItem);
    void AddItemToPlayList(const CFileItemPtr &pItem, CFileItemList &queuedItems);

  private:
    void AddFilters();

    bool IsVideoContainer(CFileItemPtr item=CFileItemPtr()) const;
    bool IsMusicContainer() const;
    bool IsPhotoContainer() const;

    int ContainerPlaylistType() const
    {
      int currentPlaylist = PLAYLIST_NONE;
      if (IsVideoContainer()) currentPlaylist = PLAYLIST_VIDEO;
      else if (IsMusicContainer()) currentPlaylist = PLAYLIST_MUSIC;
      else if (IsPhotoContainer()) currentPlaylist = PLAYLIST_PICTURE;
      return currentPlaylist;
    }

    void LoadPage(int start, int numberOfItems);
    void LoadNextPage();
    virtual void OnJobComplete(unsigned int jobID, bool success, CJob *job);
    void updateFilterButtons(CPlexSectionFilterPtr filter, bool clear=false, bool disable=false);
    void ToggleClearFilterButton(bool onOff);

    bool m_returningFromSkinLoad;
    int m_pagingOffset;
    int m_currentJobId;
    CURL m_sectionRoot;
    void UpdateSectionTitle();
    bool m_hasAdvancedFilters;

    CCriticalSection m_filterValuesSection;
    std::string m_waitingForFilter;
    CEvent m_filterValuesEvent;
    CGUIButtonControl *m_clearFilterButton;

    std::map<std::string, int> m_lastSelectedIndex;

    CPlexNavigationHelper m_navHelper;
    CURL GetUrlWithParentArgument(const CURL &originalUrl);
};

class CGUIPlexMusicWindow : public CGUIPlexMediaWindow
{
  public:
    CGUIPlexMusicWindow() : CGUIPlexMediaWindow(WINDOW_MUSIC_FILES, "MyMusicSongs.xml") {}
};

class CGUIPlexPictureWindow : public CGUIPlexMediaWindow
{
  public:
    CGUIPlexPictureWindow() : CGUIPlexMediaWindow(WINDOW_PICTURES, "MyPics.xml") {}
};
#endif // GUIWINDOWMEDIAFILTERVIEW_H
