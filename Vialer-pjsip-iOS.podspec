Pod::Spec.new do |s|
	s.name                  = "Vialer-pjsip-iOS"
	s.version               = "4.1"
	s.summary               = "PJSIP 2.10 for iOS"
	s.description           = "Pod for distributing PJSIP 2.10 wrapped in a framework. With Opus, OpenSSL and openh264."
	s.homepage              = "https://www.wearespindle.com"

	s.license               = { :type => "GNU GPLv3", :file => "LICENSE" }
	s.author                = { "Devhouse Spindle" => "opensource@wearespindle.com" }

	s.source                = { :git => "https://github.com/RickPickle/Vialer-pjsip-iOS.git", :tag => s.version.to_s }
	s.social_media_url      = "https://twitter.com/wearespindle"

	s.platform              = :ios, "10.0"
	s.requires_arc          = false

	s.preserve_paths        = "VialerPJSIP.framework/*"
	s.public_header_files   = "VialerPJSIP.framework/Versions/A/Headers/**/*.{h,hpp}"
	s.source_files		    = "VialerPJSIP.framework/Versions/A/Headers/**/*.{h,hpp}"
	s.vendored_frameworks   = "VialerPJSIP.framework"

	s.libraries 			= 'stdc++'
	s.frameworks            = "AudioToolbox", "AVFoundation", "CFNetwork",  "CoreMedia"
	s.xcconfig = {
        'GCC_PREPROCESSOR_DEFINITIONS' => 'PJ_AUTOCONF=1',
	}
end

