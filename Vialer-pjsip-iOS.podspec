Pod::Spec.new do |s|
	s.name                  = "Vialer-pjsip-iOS"
	s.version               = "3.3.4"
	s.summary               = "PJSIP 2.7.1 for iOS"
	s.description           = "Pod for distributing PJSIP 2.7.1 wrapped in a framework. With Opus, OpenSSL and openh264."
	s.homepage              = "https://www.wearespindle.com"

	s.license               = { :type => "GNU GPLv3", :file => "LICENSE" }
	s.author                = { "Devhouse Spindle" => "vialersiplib@wearespindle.com" }

	s.source                = { :git => "https://github.com/VoIPGRID/Vialer-pjsip-iOS.git", :tag => s.version.to_s }
	s.social_media_url      = "https://twitter.com/wearespindle"

	s.platform              = :ios, "9.0"
	s.requires_arc          = false

	s.preserve_paths        = "VialerPJSIP.framework/*"
	s.public_header_files   = "VialerPJSIP.framework/Versions/A/Headers/**/*.{h,hpp}"
	s.source_files		    = "VialerPJSIP.framework/Versions/A/Headers/**/*.{h,hpp}"
	s.vendored_frameworks   = "VialerPJSIP.framework"
	
	s.libraries 			= 'stdc++'
	s.frameworks            = "AudioToolbox", "AVFoundation", "CFNetwork",  "CoreMedia"

	s.script_phase = { 
		:name => "Combine VialerPJSIP library in one file", 
		:execution_position => :after_compile,
		:script => '
		BINARY_FILENAME=VialerPJSIP
		SPLIT_FILENAME_PREFIX=${BINARY_FILENAME}_Split
		VIALERPJSIP_FRAMEWORK_DIR=${SRCROOT}/Vialer-pjsip-iOS/VialerPJSIP.framework/Versions/Current
		VIALERPJSIP_FRAMEWORK_FILE=$VIALERPJSIP_FRAMEWORK_DIR/$BINARY_FILENAME
		
		minimumsize=50000000
		actualsize=$(wc -c <"${VIALERPJSIP_FRAMEWORK_FILE}")
		
		if [[ -f "${VIALERPJSIP_FRAMEWORK_FILE}" && $actualsize -ge $minimumsize ]]; then
			echo "The library is already combined"
		else
			cd ${VIALERPJSIP_FRAMEWORK_DIR}
			#  But we have chunks! ...Probably
			if [ -f ${SPLIT_FILENAME_PREFIX}_aa ]; then
				echo "Creating file from smaller files with ${SPLIT_FILENAME_PREFIX} prefix"
				cat ${SPLIT_FILENAME_PREFIX}_* > ${BINARY_FILENAME}
			    rm ${SPLIT_FILENAME_PREFIX}_*
			fi
		fi		
		'
	}
	s.xcconfig = {
        'GCC_PREPROCESSOR_DEFINITIONS' => 'PJ_AUTOCONF=1',
	}
end

