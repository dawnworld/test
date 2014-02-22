# Note that this is NOT a relocatable package
# $Id: wireshark.spec.in 48612 2013-03-28 14:17:38Z morriss $
# packaging/rpm/SPECS/wireshark.spec.  Generated from wireshark.spec.in by configure.

# Set these to 1 if you want to ensure your package inclues support for them:
%global with_adns 0
%global with_c_ares 1
%global with_lua  1
%global with_portaudio 0

# Set to 1 if you want GTK3 instead of GTK2:
%global with_gtk3 0

# Set at most one of these two:
# Note that setcap requires rpmbuild 4.7.0 or later.
%global setuid_dumpcap 0
%global setcap_dumpcap 1

# Set to 1 if you want a group called 'wireshark' which users must be a member
# of in order to run dumpcap.  Only used if setuid_dumpcap or setcap_dumpcap
# are set.
%global use_wireshark_group 1


Summary:	Network protocol analyzer
Name:		wireshark
Version:	1.10.5
Release:	1
License:	GPL+
Group:		Applications/Internet
Source:		http://wireshark.org/download/src/%{name}-%{version}.tar.bz2
# Or this URL for automated builds:
#Source:	http://wireshark.org/download/automated/src/%{name}-%{version}.tar.bz2
URL:		http://www.wireshark.org/
BuildRoot:	/tmp/wireshark-%{PACKAGE_VERSION}-root
Packager:	Gerald Combs <gerald[AT]wireshark.org>
# Some distributions create a wireshark-devel package; get rid of it
Obsoletes:	wireshark-devel

BuildRequires:	autoconf >= 2.60
BuildRequires:	automake
BuildRequires:	libtool
BuildRequires:	gcc
BuildRequires:	flex
BuildRequires:	bison
BuildRequires:	python
BuildRequires:	perl

BuildRequires:	glib2-devel >= 2.14.0
Requires:	glib2 >= 2.14.0
BuildRequires:	libpcap-devel
Requires:	libpcap
BuildRequires:	zlib-devel
Requires:	zlib

%if %{with_c_ares}
%if 0%{?suse_version}
# SuSE uses these package names (yes 2!):
BuildRequires:	libcares-devel
Requires:	libcares2
%else
# ... while Redhat uses this one:
# (What other RPM-based distros do will have to be determined...)
BuildRequires:	c-ares-devel
Requires:	c-ares
%endif
%endif
%if %{with_adns}
BuildRequires:	adns-devel
Requires:	adns
%endif
%if %{with_lua}
BuildRequires:	lua-devel
Requires:	lua
%endif

# Uncomment these if you want to be sure you get them...
#BuildRequires:	krb5-devel
#BuildRequires:	libsmi-devel
#BuildRequires:	pcre-devel
#BuildRequires:	libselinux
#BuildRequires:	gnutls-devel
#BuildRequires:	libcap-devel

%if %{use_wireshark_group}
%if 0%{?suse_version}
# SuSE's groupadd is in this package:
Requires(pre):	pwdutils
%else
# ... while Redhat's is in this one:
Requires(pre):	shadow-utils
%endif
%endif
Requires(post):	desktop-file-utils

%if %{setcap_dumpcap}
# Actually we require rpmbuild (the program) >= 4.7.0 but the package name
# where we can find it varies.  So we check the 'rpm' version because either
# rpmbuild is in that package (e.g., in older distros) or it's in the
# 'rpm-build' package which generally requires a matching version of 'rpm'.
#
# All of this is to save users the trouble of getting through an full compile
# only to have rpmbuild barf because it doesn't understand capabilities.
BuildRequires:	rpm >= 4.7.0
%endif


%package	gnome
Summary:	Gnome desktop integration for wireshark
Group:		Applications/Internet
%if %{with_gtk3}
Requires:	gtk3 >= 3.0.0
BuildRequires:	gtk3-devel >= 3.0.0
%else
Requires:	gtk2 >= 2.12.0
BuildRequires:	gtk2-devel >= 2.12.0
%endif
Requires:	wireshark = %{version}-%{release}
Requires:	xdg-utils
Requires:	hicolor-icon-theme
BuildRequires:	desktop-file-utils
%if 0%{?suse_version}
# Need this for SuSE's suse_update_desktop_file macro
BuildRequires:	update-desktop-files
%endif
%if %{with_portaudio}
BuildRequires:	portaudio-devel
Requires:	portaudio
%endif

# Uncomment these if you want to be sure you get them...
#BuildRequires:	GeoIP-devel
#Requires:	GeoIP
# Add this for more readable fonts on some distributions/versions
#Requires:	dejavu-sans-mono-fonts


%description
Wireshark is a free network protocol analyzer for Unix and Windows. It
allows you to examine data from a live network or from a capture file
on disk. You can interactively browse the capture data, viewing summary
and detail information for each packet. Wireshark has several powerful
features, including a rich display filter language and the ability to
view the reconstructed stream of a TCP session.

This package contains command-line utilities, plugins, and documentation for
Wireshark. A GTK+ graphical user interface is packaged separately.

%description gnome
Contains the Gnome (GTK+) Wireshark GUI and desktop integration files.

%prep
%setup -q -n %{name}-%{version}

# Don't specify the prefix here: configure is a macro which expands to set
# the prefix and everything else too.  If you need to change the prefix
# set _prefix (note the underscore) either in this file or on rpmbuild's
# command-line.
%configure \
  --with-gnu-ld \
%if %{with_c_ares}
  --with-c-ares \
%endif
%if %{with_adns}
  --with-adns \
%endif
%if %{with_lua}
  --with-lua \
%endif
%if %{with_portaudio}
  --with-portaudio \
%endif
%if %{with_gtk3}
  --with-gtk3 \
%endif
  --disable-warnings-as-errors

# Suggestion: put this in your ~/.rpmmacros (without the hash sign, of course):
# %_smp_mflags -j %(grep -c processor /proc/cpuinfo)
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
make DESTDIR=$RPM_BUILD_ROOT install_desktop_files
%if 0%{?suse_version}
# SuSE's packaging conventions
# (http://en.opensuse.org/openSUSE:Packaging_Conventions_RPM_Macros#.25suse_update_desktop_file)
# require this:
%suse_update_desktop_file %{name}
%else
# Fedora's packaging guidelines (https://fedoraproject.org/wiki/Packaging:Guidelines)
# require this (at least if desktop-file-install was not used to install it).
desktop-file-validate %{buildroot}/usr/share/applications/wireshark.desktop
%endif

%clean
rm -rf $RPM_BUILD_ROOT

%if %{use_wireshark_group}
%pre
getent group wireshark >/dev/null || groupadd -r wireshark
%endif

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%post gnome
update-desktop-database &> /dev/null ||:
update-mime-database /usr/share/mime &> /dev/null || :
touch --no-create /usr/share/icons/hicolor &>/dev/null || :

%postun gnome
update-desktop-database &> /dev/null ||:
update-mime-database /usr/share/mime &> /dev/null || :
if [ $1 -eq 0 ] ; then
	touch --no-create /usr/share/icons/hicolor &>/dev/null || :
	gtk-update-icon-cache /usr/share/icons/hicolor &>/dev/null || :
fi

# Is this really needed?
%posttrans
gtk-update-icon-cache /usr/share/icons/hicolor &>/dev/null || :


%files
%defattr(-,root,root)
%doc AUTHORS COPYING ChangeLog INSTALL INSTALL.configure NEWS README*
# Don't pick up the wireshark (GUI) binary here
%exclude %{_bindir}/wireshark
%{_bindir}/*

# This generates a warning because dumpcap is listed twice. That's
# probably preferable to listing each program (and keeping the list up to
# date)...  Maybe if we can find a way to get the toplevel Makefile.am's
# bin_PROGRAMS in here?
%if %{use_wireshark_group} && %{setuid_dumpcap}
# Setuid but only executable by members of the 'wireshark' group
%attr(4750, root, wireshark) %{_bindir}/dumpcap
%else
%if %{use_wireshark_group} && %{setcap_dumpcap}
# Setcap but only executable by members of the 'wireshark' group
%attr(0750, root, wireshark) %caps(cap_net_raw,cap_net_admin=eip) %{_bindir}/dumpcap
%else
%if %{setuid_dumpcap}
# Setuid and executable by all
%attr(4755, root, root) %{_bindir}/dumpcap
%else
%if %{setcap_dumpcap}
# Setcap and executable by all
%attr(0755, root, root) %caps(cap_net_raw,cap_net_admin=eip) %{_bindir}/dumpcap
%else
# Executable by all but with no special permissions
%attr(0755, root, root) %{_bindir}/dumpcap
%endif
%endif
%endif
%endif

%{_libdir}/lib*.so*
%exclude %{_libdir}/%{name}/plugins/%{version}/*.la
%exclude %{_libdir}/*.la
%{_libdir}/wireshark
# Don't pick up the wireshark (GUI) man page
%exclude %{_mandir}/man1/wireshark.*
%{_mandir}/man1/*
%{_mandir}/man4/*
%{_datadir}/wireshark

%files gnome
%defattr(-,root,root)
/usr/share/applications/wireshark.desktop
/usr/share/icons/hicolor/*/apps/*
/usr/share/icons/hicolor/*/mimetypes/*
/usr/share/mime/packages/wireshark.xml
%{_bindir}/wireshark
%{_mandir}/man1/wireshark.*

%changelog
* Thu Mar 28 2013 Jeff Morriss
- Simplify check for rpmbuild's version.

* Fri Mar  8 2013 Jeff Morriss
- Put all icons in hicolor
- Use SuSE's desktop-update macro.
- Actually update MIME database when Wireshark's prefix is not /usr .

* Thu Mar  6 2013 Jeff Morriss
- List more build dependencies.
- Update to work on SuSE too: some of their package names are different.

* Thu Mar  6 2013 Gerald Combs
- Enable c-ares by default

* Mon Feb  6 2013 Jeff Morriss
- Overhaul to make this file more useful/up to date.  Many changes are based
  on Fedora's .spec file.  Changes include:
  - Create a separate wireshark-gnome package (like Redhat).
  - Control some things with variables set at the top of the file.
    - Allow the user to configure how dumpcap is installed.
    - Allow the user to choose some options including GTK2 or GTK3.
  - Greatly expand the BuildRequires entries; get the minimum versions of some
    things from 'configure'.
  - Install freedesktop files for better (free)desktop integration.

* Thu Aug 10 2006 Joerg Mayer
- Starting with X.org 7.x X11R6 is being phased out. Install wireshark
  and manpage into the standard path.

* Mon Aug 01 2005 Gerald Combs
- Add a desktop file and icon for future use

- Take over the role of packager

- Update descriptions and source locations

* Thu Oct 28 2004 Joerg Mayer
- Add openssl requirement (heimdal and net-snmp are still automatic)

* Tue Jul 20 2004 Joerg Mayer
- Redo install and files section to actually work with normal builds

* Sat Feb 07 2004 Joerg Mayer
- in case there are shared libs: include them

* Tue Aug 24 1999 Gilbert Ramirez
- changed to ethereal.spec.in so that 'configure' can update
  the version automatically

* Tue Aug 03 1999 Gilbert Ramirez <gram@xiexie.org>
- updated to 0.7.0 and changed gtk+ requirement

* Sun Jan 01 1999 Gerald Combs <gerald@zing.org>
- updated to 0.5.1

* Fri Nov 20 1998 FastJack <fastjack@i-s-o.net>
- updated to 0.5.0

* Sun Nov 15 1998 FastJack <fastjack@i-s-o.net>
- created .spec file
