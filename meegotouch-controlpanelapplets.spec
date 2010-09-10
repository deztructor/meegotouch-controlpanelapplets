Name:           meegotouch-controlpanelapplets
Version:        0.15.2
Release:        1%{?dist}
Summary:        Various MeeGo Touch Controlpanel Applets

Group:          System/GUI/Other
License:        LGPL v2.1
URL:            http://meego.gitorious.org/meegotouch/meegotouch-controlpanelapplets

Source0:        %{name}-%{version}.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: pkgconfig(meegotouch) >= 0.20.37
BuildRequires: pkgconfig(QtCore) >= 4.7.0
BuildRequires: meegotouch-controlpanel-devel
BuildRequires: libqttracker-devel

BuildRequires: qt-devel
# this is for profile-plugin:
BuildRequires: meegotouch-systemui-devel
# TODO: clean-device, libthumnailer is not yet packaged for MeeGo

%description
Various Controlpanel applets for MeeGo Touch environment:
    - Offline switcher
    - USB settings
    - Battery info/settings
    - Display settings
    - Profiles settings
    - Theme-switcher
    - Wallpaper editor/setter
    - Clear-Device & Restore-Factory-Settings
    - About product
    - Warranty applet
    - Profiles switcher

%package -n meegotouchcp-applets-l10n-eng-en
Summary: MeeGo Touch Controlpanel applets engineering English messages
Provides: meegotouchcp-applets-l10n
%description -n meegotouchcp-applets-l10n-eng-en
MeeGo Touch Controlpanel applets engineering English messages for various applets:
    - Offline switcher
    - USB settings
    - Battery info/settings
    - Display settings
    - Profiles settings
    - Theme-switcher
    - Wallpaper editor/setter
    - Clear-Device & Restore-Factory-Settings
    - About product
    - Warranty applet
    - Profiles switcher

%package -n meegotouchcp-offline
Summary: MeegoTouch Controlpanel Offline switcher
%description -n meegotouchcp-offline
MeegoTouch Controlpanel Offline switcher

%package -n meegotouchcp-usb
Summary: MeegoTouch Controlpanel USB settings
%description -n meegotouchcp-usb
MeegoTouch Controlpanel USB settings

%package -n meegotouchcp-battery
Summary: MeegoTouch Controlpanel battery info/settings
%description -n meegotouchcp-battery
MeegoTouch Controlpanel battery info/settings

%package -n meegotouchcp-display
Summary: MeegoTouch Controlpanel display settings
%description -n meegotouchcp-display
MeegoTouch Controlpanel display settings

%package -n meegotouchcp-profiles
Summary: MeegoTouch Controlpanel profiles settings
%description -n meegotouchcp-profiles
MeegoTouch Controlpanel profiles settings

%package -n meegotouchcp-theme
Summary: MeegoTouch Controlpanel theme switcher
%description -n meegotouchcp-theme
MeegoTouch Controlpanel theme switcher

%package -n meegotouchcp-wallpaper
Summary: MeegoTouch Controlpanel wallpaper editor & setter
Requires: tumbler
%description -n meegotouchcp-wallpaper
MeegoTouch Controlpanel wallpaper editor & setter

%package -n meegotouchcp-reset
Summary: MeegoTouch Controlpanel reset-settings & clear-user data
%description -n meegotouchcp-reset
MeegoTouch Controlpanel reset-settings & clear-user data

%package -n meegotouchcp-about
Summary: MeegoTouch Controlpanel about product applet
%description -n meegotouchcp-about
MeegoTouch Controlpanel about product applet

%package -n meegotouchcp-warranty
Summary: MeegoTouch Controlpanel warranty applet
%description -n meegotouchcp-warranty
MeegoTouch Controlpanel warranty applet

%package -n meego-handset-statusindicators-profiles
Summary: MeegoTouch profile-switcher statusindicatormenu plugin
Requires: meegotouch-systemui
%description -n meego-handset-statusindicators-profiles
MeegoTouch profile-switcher statusindicatormenu plugin

%prep
%setup -q -n %{name}-%{version}

%build
qmake
make %{?_smp_mflags}

%install
rm -fR %{buildroot}
make INSTALL_ROOT=%{buildroot} install
install -m 644 -D -p LICENSE %{buildroot}/usr/share/doc/packages/%{name}/LICENSE.LGPL

%clean
rm -fR %{buildroot}

%files
%defattr(-,root,root)
%dir /usr/share/doc/packages/%{name}
%doc /usr/share/doc/packages/%{name}/*

%files -n meegotouchcp-applets-l10n-eng-en
%defattr(-,root,root)
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/systemui-applets.qm

%files -n meegotouchcp-offline
%defattr(-,root,root)
%dir /usr/lib/duicontrolpanel
%dir /usr/lib/duicontrolpanel/applets
/usr/lib/duicontrolpanel/offline.desktop
/usr/lib/duicontrolpanel/applets/libofflineapplet.so

%files -n meegotouchcp-usb
%defattr(-,root,root)
%dir /usr/lib/duicontrolpanel
%dir /usr/lib/duicontrolpanel/applets
/usr/lib/duicontrolpanel/usbapplet.desktop
/usr/lib/duicontrolpanel/applets/libusbapplet.so

%files -n meegotouchcp-battery
%defattr(-,root,root)
%dir /usr/lib/duicontrolpanel
%dir /usr/lib/duicontrolpanel/applets
%dir /usr/share/themes/base/meegotouch/duicontrolpanel/style
/usr/lib/duicontrolpanel/battery.desktop
/usr/lib/duicontrolpanel/applets/libbatteryapplet.so
/usr/share/themes/base/meegotouch/duicontrolpanel/style/batteryapplet.css

%files -n meegotouchcp-display
%defattr(-,root,root)
%dir /usr/lib/duicontrolpanel
%dir /usr/lib/duicontrolpanel/applets
%dir /usr/share/themes/base/meegotouch/duicontrolpanel/style
/usr/lib/duicontrolpanel/display.desktop
/usr/lib/duicontrolpanel/applets/libdisplayapplet.so
/usr/share/themes/base/meegotouch/duicontrolpanel/style/display.css

%files -n meegotouchcp-profiles
%defattr(-,root,root)
%dir /usr/lib/duicontrolpanel
%dir /usr/lib/duicontrolpanel/applets
%dir /usr/share/themes/base/meegotouch/duicontrolpanel/style
/usr/lib/duicontrolpanel/profile.desktop
/usr/lib/duicontrolpanel/applets/libprofileapplet.so
/usr/share/themes/base/meegotouch/duicontrolpanel/style/profileapplet.css

%files -n meegotouchcp-theme
%defattr(-,root,root)
%dir /usr/lib/duicontrolpanel
%dir /usr/lib/duicontrolpanel/applets
%dir /usr/share/themes/base/meegotouch/duicontrolpanel/style
/usr/lib/duicontrolpanel/theme.desktop
/usr/lib/duicontrolpanel/applets/libthemeapplet.so
/usr/share/themes/base/meegotouch/duicontrolpanel/style/themeapplet.css

%files -n meegotouchcp-wallpaper
%defattr(-,root,root)
%dir /usr/lib/duicontrolpanel
%dir /usr/lib/duicontrolpanel/applets
%dir /usr/share/themes/base/meegotouch/duicontrolpanel/style
/usr/lib/duicontrolpanel/wallpaper.desktop
/usr/lib/duicontrolpanel/applets/libwallpaperapplet.so
/usr/share/themes/base/meegotouch/duicontrolpanel/style/wallpaper.css

%files -n meegotouchcp-reset
%defattr(-,root,root)
%dir /usr/lib/duicontrolpanel
%dir /usr/lib/duicontrolpanel/applets
/usr/lib/duicontrolpanel/reset.desktop
/usr/lib/duicontrolpanel/applets/libresetapplet.so

%files -n meegotouchcp-about
%defattr(-,root,root)
%dir /usr/lib/duicontrolpanel
%dir /usr/lib/duicontrolpanel/applets
/usr/lib/duicontrolpanel/about.desktop
/usr/lib/duicontrolpanel/applets/libaboutapplet.so

%files -n meegotouchcp-warranty
%defattr(-,root,root)
%dir /usr/lib/duicontrolpanel
%dir /usr/lib/duicontrolpanel/applets
/usr/lib/duicontrolpanel/warranty.desktop
/usr/lib/duicontrolpanel/applets/libwarrantyapplet.so

%files -n meego-handset-statusindicators-profiles
%defattr(-,root,root)
%dir /usr/lib/meegotouch/applicationextensions
/usr/lib/meegotouch/applicationextensions/libstatusindicatormenu-profile.so
/usr/share/meegotouch/applicationextensions/statusindicatormenu-profile.desktop

%changelog
