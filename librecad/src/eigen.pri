
# Necessary eigen3 libraries

exists( custom.pro ):include( custom.pro )
exists( custom.pri ):include( custom.pri )

unix {
!macx {
	# linux
	CONFIG += link_pkgconfig
	PKGCONFIG += eigen3
	message("setting eigen3")
}
}

