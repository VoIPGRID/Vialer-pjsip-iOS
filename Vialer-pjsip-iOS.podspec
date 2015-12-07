Pod::Spec.new do |s|
	s.name                  = "Vialer-pjsip-iOS"
	s.version               = "0.0.1"
	s.summary               = "PJSIP 2.4.5 for iOS"
	s.description           = <<-DESC
                                    Pod for distributing PJSIP wrapped in a framework.
                                DESC
	
	s.homepage              = "https://www.wearespindle.com"
	
	s.license               = { :type => "MIT", :file => "LICENSE" }
	s.author                = { "Devhouse Spindle" => "hello@wearespindle.com" }
	
	s.source                = { :git => "https://github.com/VoIPGRID/Vialer-pjsip-iOS.git", :tag => s.version.to_s }
	s.social_media_url      = "https://twitter.com/wearespindle"

	s.platform              = :ios, "9.0"
	s.requires_arc          = false

        #s.preserve_paths        = "VialerPJSIP.framework/*"
        #s.public_header_files   = "VialerPJSIP.framework/Versions/A/Headers/**/*.{h,hpp}"
        s.vendored_frameworks   = "VialerPJSIP.framework"

        s.frameworks = "AudioToolbox", "AVFoundation", "CFNetwork", "CoreMedia", "CoreVideo", "CoreAudio", "Foundation"

  	s.xcconfig = {
            'GCC_PREPROCESSOR_DEFINITIONS' => 'PJ_AUTOCONF=1'
  	}
end