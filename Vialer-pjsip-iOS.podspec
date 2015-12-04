Pod::Spec.new do |s|
	s.name             = "Vialer-pjsip-iOS"
	s.version          = "0.0.1"
	s.summary          = "PJSIP 2.4.5 for iOS"
	s.description      = <<-DESC
							Other pods for pjsip wrapper were not doing wat we needed. pjsip shouldn"t stay connected all the time.
						DESC
	
	s.homepage         = "https://www.wearespindle.com"
	
	s.license          = { :type => "MIT", :file => "LICENSE" }
	s.author           = { "Devhouse Spindle" => "hello@wearespindle.com" }
	
	s.source           = { :git => "https://github.com/VoIPGRID/Vialer-pjsip-iOS.git", :tag => s.version.to_s }
	s.social_media_url = "https://twitter.com/wearespindle"

	s.platform     = :ios, "9.0"
	s.requires_arc = false

	s.public_header_files = "Pod/vialer-pjsip-include/**/*.{h,hpp}"
	s.preserve_paths = "Pod/vialer-pjsip-include/**/**/*.{h,hpp}"
	s.header_mappings_dir = "Pod"
					 
  	s.vendored_libraries = "Pod/libVialer-pjsip.a"

  	# We wanted to ommit the "HEADER_SEARCH_PATH" line in s.xcconfig but this is not possible.
  	# You cannot include the PJSIP Header files in a way that it's directory structure is preserved.
  	# s.header_mappings_dir should do this but doesn't 	
  	# s.source_files = "Pod/vialer-pjsip-include/**/**"
  	
  	s.frameworks = "AudioToolbox", "AVFoundation", "CFNetwork", "CoreMedia", "CoreVideo", "CoreAudio", "Foundation"
  	
  	s.xcconfig = {
    	"GCC_PREPROCESSOR_DEFINITIONS" => "PJ_AUTOCONF=1",
    	"HEADER_SEARCH_PATHS"  => "$(PODS_ROOT)/Vialer-pjsip-iOS/Pod/vialer-pjsip-include"
  	}
end