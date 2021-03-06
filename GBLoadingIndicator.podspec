Pod::Spec.new do |s|
  s.name         = 'GBLoadingIndicator'
  s.version      = '1.0.3'
  s.summary      = 'A tiny library for quickly adding custom loading indicators to your own views, that hides the complexity of having to manage state yourself.'
  s.homepage     = 'https://github.com/lmirosevic/GBLoadingIndicator'
  s.license      = 'Apache License, Version 2.0'
  s.author       = { 'Luka Mirosevic' => 'luka@goonbee.com' }
  s.platform     = :ios, '5.0'
  s.source       = { git: 'https://github.com/lmirosevic/GBLoadingIndicator.git', tag: s.version.to_s }
  s.source_files  = 'GBLoadingIndicator'
  s.public_header_files = 'GBLoadingIndicator/GBLoadingIndicator.h', 'GBLoadingIndicator/GBLoadingIndicatorConcrete.h', 'GBLoadingIndicator/GBLoadingIndicator_Types.h', 'GBLoadingIndicator/GBLoadingIndicator_StandardHandlers.h'
  s.requires_arc = true

  s.dependency 'GBToolbox'
end
