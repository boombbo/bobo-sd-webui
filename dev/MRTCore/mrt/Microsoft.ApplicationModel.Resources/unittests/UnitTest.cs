﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using WEX.TestExecution;
using WEX.TestExecution.Markup;
using Microsoft.ApplicationModel.Resources;

namespace ManagedTest
{
    [TestClass]
    public class ResourceLoaderTest
    {
        [TestMethod]
        public void GetStringTest()
        {
            var resourceLoader = new ResourceLoader("resources.pri.standalone");
            var resource = resourceLoader.GetString("IDS_MANIFEST_MUSIC_APP_NAME");
            Verify.AreEqual(resource, "Groove Music");
        }

        [TestMethod]
        public void GetStringTest_NonDefaultNamespace()
        {
            var resourceLoader = new ResourceLoader("resources.pri.standalone", "Microsoft.UI.Xaml/Resources");
            var resource = resourceLoader.GetString("HelpTextMoreButton");
            Verify.AreEqual(resource, "Invoke to show or hide the text entry fields.");
        }

        [TestMethod]
        public void GetStringForUriTest()
        {
            var resourceLoader = new ResourceLoader("resources.pri.standalone");
            var resource = resourceLoader.GetStringForUri(new Uri("ms-resource://Microsoft.ZuneMusic/resources/IDS_MANIFEST_MUSIC_APP_NAME"));
            Verify.AreEqual(resource, "Groove Music");
        }

        [TestMethod]
        public void GetStringForUriTest_ImplicitRootNamespace()
        {
            var resourceLoader = new ResourceLoader("resources.pri.standalone");
            var resource = resourceLoader.GetStringForUri(new Uri("ms-resource:///resources/IDS_MANIFEST_MUSIC_APP_NAME"));
            Verify.AreEqual(resource, "Groove Music");
        }

        [TestMethod]
        public void GetDefaultResourceFilePathTest()
        {
            var filepath = ResourceLoader.GetDefaultResourceFilePath();
            Verify.AreNotEqual(filepath.IndexOf("resources.pri"), -1);
        }
    }

    [TestClass]
    public class ResourceManagerTest
    {
        [TestMethod]
        public void ValueAsStringTest_StringResource_Succeeds()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");
            var stringResourceCandidate = resourceManager.MainResourceMap.GetValue("resources/IDS_MANIFEST_MUSIC_APP_NAME");
            Verify.AreEqual(stringResourceCandidate.ValueAsString, "Groove Music");
            var ex = Verify.Throws<Exception>(() => { var temp = stringResourceCandidate.ValueAsBytes; });
            Verify.AreEqual((uint)ex.HResult, 0x80073b0d); // HRESULT_FROM_WIN32(ERROR_MRM_RESOURCE_TYPE_MISMATCH)
            stringResourceCandidate = resourceManager.MainResourceMap.TryGetValue("resources/IDS_MANIFEST_MUSIC_APP_NAME");
            Verify.AreEqual(stringResourceCandidate.ValueAsString, "Groove Music");
        }

        [TestMethod]
        public void ValueAsStringTest_FileResource_Succeeds()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");
            var fileResourceCandidate = resourceManager.MainResourceMap.GetValue("Files/Assets/AppList.png");
            var fileResourceString = fileResourceCandidate.ValueAsString;
            Verify.AreNotEqual(fileResourceString.IndexOf(@"\AppList."), -1);
            fileResourceString = resourceManager.MainResourceMap.TryGetValue("Files/Assets/AppList.png").ValueAsString;
            Verify.AreNotEqual(fileResourceString.IndexOf(@"\AppList."), -1);
        }

        [TestMethod]
        public void ValueAsBlobTest_Succeeds()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");
            var resourceCandidate = resourceManager.MainResourceMap.GetValue("Files/Controls/AlbumBasicInfoControl.xbf");
            var resourceData = resourceCandidate.ValueAsBytes;
            Verify.AreEqual(resourceData.Length, 15002);
            var ex = Verify.Throws<Exception>(() => { var temp = resourceCandidate.ValueAsString; });
            Verify.AreEqual((uint)ex.HResult, 0x80073b0d); // HRESULT_FROM_WIN32(ERROR_MRM_RESOURCE_TYPE_MISMATCH)
            resourceData = resourceManager.MainResourceMap.TryGetValue("Files/Controls/AlbumBasicInfoControl.xbf").ValueAsBytes;
            Verify.AreEqual(resourceData.Length, 15002);
        }

        [TestMethod]
        public void GetKindTest()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");

            var stringResourceCandidate = resourceManager.MainResourceMap.GetValue("resources/IDS_MANIFEST_MUSIC_APP_NAME");
            Verify.AreEqual(stringResourceCandidate.Kind, ResourceCandidateKind.String);
            stringResourceCandidate = resourceManager.MainResourceMap.TryGetValue("resources/IDS_MANIFEST_MUSIC_APP_NAME");
            Verify.AreEqual(stringResourceCandidate.Kind, ResourceCandidateKind.String);

            var fileResourceCandidate = resourceManager.MainResourceMap.GetValue("Files/Assets/AppList.png");
            Verify.AreEqual(fileResourceCandidate.Kind, ResourceCandidateKind.FilePath);
            fileResourceCandidate = resourceManager.MainResourceMap.TryGetValue("Files/Assets/AppList.png");
            Verify.AreEqual(fileResourceCandidate.Kind, ResourceCandidateKind.FilePath);

            var blobResourceCandidate = resourceManager.MainResourceMap.GetValue("Files/Controls/AlbumBasicInfoControl.xbf");
            Verify.AreEqual(blobResourceCandidate.Kind, ResourceCandidateKind.EmbeddedData);
            blobResourceCandidate = resourceManager.MainResourceMap.TryGetValue("Files/Controls/AlbumBasicInfoControl.xbf");
            Verify.AreEqual(blobResourceCandidate.Kind, ResourceCandidateKind.EmbeddedData);
        }

        [TestMethod]
        public void GetSubtreeTest()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");
            var resourceMap = resourceManager.MainResourceMap.GetSubtree("resources");
            var resourceCandidate = resourceMap.GetValue("IDS_MANIFEST_MUSIC_APP_NAME");
            var resource = resourceCandidate.ValueAsString;
            Verify.AreEqual(resource, "Groove Music");

            var ex = Verify.Throws<Exception>(() => resourceMap.GetValue("abc"));
            Verify.AreEqual((uint)ex.HResult, 0x80073b17); // HRESULT_FROM_WIN32(ERROR_MRM_NAMED_RESOURCE_NOT_FOUND)
            resourceCandidate = resourceMap.TryGetValue("abc");
            Verify.IsNull(resourceCandidate);
        }

        [TestMethod]
        public void ResourceNotFoundTest()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");
            resourceManager.ResourceNotFound += (sender, args) =>
            {
                if (args.Name == "abc")
                {
                    var candidate = new ResourceCandidate(ResourceCandidateKind.String, "abcValue");
                    args.SetResolvedCandidate(candidate);
                }
            };
            var resourceMap = resourceManager.MainResourceMap.GetSubtree("resources");
            var resourceCandidate = resourceMap.GetValue("abc");
            var resource = resourceCandidate.ValueAsString;
            Verify.AreEqual(resource, "abcValue");
            resource = resourceMap.TryGetValue("abc").ValueAsString;
            Verify.AreEqual(resource, "abcValue");

            var ex = Verify.Throws<Exception>(() => resourceMap.GetValue("xyz"));
            Verify.AreEqual((uint)ex.HResult, 0x80073b17); // HRESULT_FROM_WIN32(ERROR_MRM_NAMED_RESOURCE_NOT_FOUND)
            resourceCandidate = resourceMap.TryGetValue("xyz");
            Verify.IsNull(resourceCandidate);
        }

        [TestMethod]
        public void DefaultResourceManagerTest()
        {
            var resourceManager = new ResourceManager();
            var resourceMap = resourceManager.MainResourceMap;
            var resourceChildMap = resourceMap.GetSubtree("Files");
            var resourceChildChildMap = resourceChildMap.GetSubtree("Assets");
            var resourceCandidate = resourceChildChildMap.GetValue("LockScreenLogo.png");
            Verify.AreEqual(resourceCandidate.Kind, ResourceCandidateKind.FilePath);
            Verify.AreNotEqual(resourceCandidate.ValueAsString.IndexOf(@"Assets\LockScreenLogo.scale-200.png"), -1);
            resourceCandidate = resourceChildChildMap.TryGetValue("LockScreenLogo.png");
            Verify.AreEqual(resourceCandidate.Kind, ResourceCandidateKind.FilePath);
            Verify.AreNotEqual(resourceCandidate.ValueAsString.IndexOf(@"Assets\LockScreenLogo.scale-200.png"), -1);
        }

        [TestMethod]
        public void NoResourceFileTest()
        {
            var resourceManager = new ResourceManager("NoSuchFile.pri");
            var resourceMap = resourceManager.MainResourceMap;
            var resourceChildMap = resourceMap.GetSubtree("anyname");
            var resourceContext = resourceManager.CreateResourceContext();
            Verify.IsTrue(resourceContext.QualifierValues.ContainsKey(KnownResourceQualifierName.Language));

            // No resource file, and not handled by fallback 
            var ex = Verify.Throws<Exception>(() => resourceMap.GetValue("abc"));
            Verify.AreEqual((uint)ex.HResult, 0x80070490); // HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
            var resourceCandidate = resourceMap.TryGetValue("abc");
            Verify.IsNull(resourceCandidate);

            // add fallback handler
            resourceManager.ResourceNotFound += (sender, args) =>
            {
                if (args.Name == "abc")
                {
                    var candidate = new ResourceCandidate(ResourceCandidateKind.String, "abcValue");
                    args.SetResolvedCandidate(candidate);
                }
            };

            resourceCandidate = resourceMap.GetValue("abc");
            Verify.AreEqual(resourceCandidate.Kind, ResourceCandidateKind.String);
            Verify.AreEqual(resourceCandidate.ValueAsString, "abcValue");

            resourceCandidate = resourceMap.TryGetValue("abc");
            Verify.AreEqual(resourceCandidate.Kind, ResourceCandidateKind.String);
            Verify.AreEqual(resourceCandidate.ValueAsString, "abcValue");

            ex = Verify.Throws<Exception>(() => resourceMap.GetValue("xyz"));
            Verify.AreEqual((uint)ex.HResult, 0x80070490); // HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
            resourceCandidate = resourceMap.TryGetValue("xyz");
            Verify.IsNull(resourceCandidate);
        }

        [TestMethod]
        public void ResourceEnumTest()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");
            var resourceMapXaml = resourceManager.MainResourceMap.GetSubtree("Microsoft.UI.Xaml");
            var resourceMapResources = resourceMapXaml.GetSubtree("Resources");

            var count = resourceMapResources.ResourceCount;
            Verify.AreEqual(count, 78u);

            // first resource under the resource map
            var value = resourceMapResources.GetValueByIndex(0);
            Verify.AreEqual(value.Key, "AutomationNameAlphaSlider");
            var candidate = value.Value;
            Verify.AreEqual(candidate.ValueAsString, "Opacity");
            Verify.AreEqual(candidate.Kind, ResourceCandidateKind.String);

            // last resource under the resource map
            value = resourceMapResources.GetValueByIndex(77);
            Verify.AreEqual(value.Key, "ValueStringValueSliderWithoutColorName");
            candidate = value.Value;
            Verify.AreEqual(candidate.ValueAsString, "%1!u!");
            Verify.AreEqual(candidate.Kind, ResourceCandidateKind.String);
        }
    }

    [TestClass]
    public class ResourceContextTest
    {
        [TestMethod]
        public void LanguageContextTest()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");
            var resourceContext = resourceManager.CreateResourceContext();
            var qualifierValues = resourceContext.QualifierValues;

            var resourceCandidate = resourceManager.MainResourceMap.GetValue("resources/IDS_WHATS_NEW_1710_2_EQUALIZER_TITLE", resourceContext);
            var resource = resourceCandidate.ValueAsString;
            if ((qualifierValues[KnownResourceQualifierName.Language].IndexOf("en-US") != -1) || (qualifierValues[KnownResourceQualifierName.Language].IndexOf("en") == -1))
            {
                // en-US or non-en which will match default
                Verify.AreEqual(resource, "Equalizer");
            }

            Verify.AreEqual(qualifierValues[KnownResourceQualifierName.Scale], "");

            qualifierValues[KnownResourceQualifierName.Language] = "en-GB";
            resourceCandidate = resourceManager.MainResourceMap.GetValue("resources/IDS_WHATS_NEW_1710_2_EQUALIZER_TITLE", resourceContext);
            resource = resourceCandidate.ValueAsString;
            Verify.AreEqual(resource, "Equaliser");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "EN-GB");
            Verify.IsFalse(resourceCandidate.QualifierValues.ContainsKey(KnownResourceQualifierName.Scale));

            resourceCandidate = resourceManager.MainResourceMap.TryGetValue("resources/IDS_WHATS_NEW_1710_2_EQUALIZER_TITLE", resourceContext);
            resource = resourceCandidate.ValueAsString;
            Verify.AreEqual(resource, "Equaliser");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "EN-GB");
            Verify.IsFalse(resourceCandidate.QualifierValues.ContainsKey(KnownResourceQualifierName.Scale));

            qualifierValues[KnownResourceQualifierName.Language] = "en-AU";
            resourceCandidate = resourceManager.MainResourceMap.GetValue("resources/IDS_WHATS_NEW_1710_2_EQUALIZER_TITLE", resourceContext);
            resource = resourceCandidate.ValueAsString;
            Verify.AreEqual(resource, "Equaliser");
            // Candidate for en-GB is selected
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "EN-GB");
        }

        [TestMethod]
        public void NonLanguageContextTest()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");
            var resourceContext = resourceManager.CreateResourceContext();
            Verify.AreEqual(resourceContext.QualifierValues[KnownResourceQualifierName.Contrast], "");
            Verify.AreEqual(resourceContext.QualifierValues[KnownResourceQualifierName.TargetSize], "");
            Verify.AreEqual(resourceContext.QualifierValues["AlternateForm"], "");

            resourceContext.QualifierValues[KnownResourceQualifierName.Contrast] = "White";
            resourceContext.QualifierValues[KnownResourceQualifierName.TargetSize] = "96";
            var resourceCandidate = resourceManager.MainResourceMap.GetValue("Files/Assets/AppList.png", resourceContext);
            var resource = resourceCandidate.ValueAsString;
            Verify.AreNotEqual(resource.IndexOf(@"Assets\contrast-white\AppList.targetsize-96_contrast-white.png"), -1);
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Contrast], "WHITE");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.TargetSize], "96");

            resourceContext.QualifierValues[KnownResourceQualifierName.Contrast] = "Black";
            resourceContext.QualifierValues[KnownResourceQualifierName.TargetSize] = "72";
            resourceContext.QualifierValues["AlternateForm"] = "UNPLATED";
            resourceCandidate = resourceManager.MainResourceMap.GetValue("Files/Assets/AppList.png", resourceContext);
            resource = resourceCandidate.ValueAsString;
            Verify.AreNotEqual(resource.IndexOf(@"Assets\contrast-black\AppList.targetsize-72_altform-unplated_contrast-black.png"), -1);
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Contrast], "BLACK");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.TargetSize], "72");
            Verify.AreEqual(resourceCandidate.QualifierValues["AlternateForm"], "UNPLATED");
        }

        [TestMethod]
        public void ResourceEnumWithContextTest()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");
            var resourceMap = resourceManager.MainResourceMap.GetSubtree("Resources");
            var resourceContext = resourceManager.CreateResourceContext();

            var count = resourceMap.ResourceCount;
            Verify.AreEqual(count, 1269u);

            resourceContext.QualifierValues[KnownResourceQualifierName.Language] = "en-US";

            // first resource under the resource map
            var value = resourceMap.GetValueByIndex(0, resourceContext);
            Verify.AreEqual(value.Key, "000D1359");
            var candidate = value.Value;
            Verify.AreEqual(candidate.ValueAsString, "This song is available only when you buy the whole album.");
            Verify.AreEqual(candidate.Kind, ResourceCandidateKind.String);
            Verify.AreEqual(candidate.QualifierValues[KnownResourceQualifierName.Language], "EN-US");

            // second to last resource under the resource map
            value = resourceMap.GetValueByIndex(1267, resourceContext);
            Verify.AreEqual(value.Key, "IDS_WHATS_NEW_1710_2_EQUALIZER_TITLE");
            candidate = value.Value;
            Verify.AreEqual(candidate.ValueAsString, "Equalizer");
            Verify.AreEqual(candidate.Kind, ResourceCandidateKind.String);

            // Change language context
            resourceContext.QualifierValues[KnownResourceQualifierName.Language] = "en-GB";

            // first resource under the resource map
            value = resourceMap.GetValueByIndex(0, resourceContext);
            Verify.AreEqual(value.Key, "000D1359");
            candidate = value.Value;
            Verify.AreEqual(candidate.ValueAsString, "This song is available only when you buy the whole album.");
            Verify.AreEqual(candidate.Kind, ResourceCandidateKind.String);
            Verify.AreEqual(candidate.QualifierValues[KnownResourceQualifierName.Language], "EN-GB");

            // second to last resource under the resource map
            value = resourceMap.GetValueByIndex(1267, resourceContext);
            Verify.AreEqual(value.Key, "IDS_WHATS_NEW_1710_2_EQUALIZER_TITLE");
            candidate = value.Value;
            Verify.AreEqual(candidate.ValueAsString, "Equaliser");
            Verify.AreEqual(candidate.Kind, ResourceCandidateKind.String);
        }

        [TestMethod]
        public void ResourceNotFoundWithContextTest()
        {
            var resourceManager = new ResourceManager("resources.pri.standalone");
            resourceManager.ResourceNotFound += (sender, args) =>
            {
                if (args.Name == "abc")
                {
                    String value;
                    if (args.Context.QualifierValues[KnownResourceQualifierName.Language].StartsWith("en-US"))
                    {
                        value = "USValue";
                    }
                    else if (args.Context.QualifierValues[KnownResourceQualifierName.Language].StartsWith("en-GB"))
                    {
                        value = "GBValue";
                    }
                    else
                    {
                        value = "OtherValue";
                    }
                    var candidate = new ResourceCandidate(ResourceCandidateKind.String, value);
                    args.SetResolvedCandidate(candidate);
                }
            };

            var resourceContext = resourceManager.CreateResourceContext();
            resourceContext.QualifierValues[KnownResourceQualifierName.Language] = "en-US";
            var resourceMap = resourceManager.MainResourceMap.GetSubtree("resources");
            var resourceCandidate = resourceMap.GetValue("abc", resourceContext);
            var resource = resourceCandidate.ValueAsString;
            Verify.AreEqual(resource, "USValue");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "en-US");

            resourceContext.QualifierValues[KnownResourceQualifierName.Language] = "en-GB";
            resourceCandidate = resourceMap.GetValue("abc", resourceContext);
            resource = resourceCandidate.ValueAsString;
            Verify.AreEqual(resource, "GBValue");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "en-GB");

            resourceCandidate = resourceMap.TryGetValue("abc", resourceContext);
            resource = resourceCandidate.ValueAsString;
            Verify.AreEqual(resource, "GBValue");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "en-GB");

            resourceContext.QualifierValues[KnownResourceQualifierName.Language] = "fr-FR";
            resourceCandidate = resourceMap.GetValue("abc", resourceContext);
            resource = resourceCandidate.ValueAsString;
            Verify.AreEqual(resource, "OtherValue");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "fr-FR");

            var ex = Verify.Throws<Exception>(() => resourceMap.GetValue("xyz", resourceContext));
            Verify.AreEqual((uint)ex.HResult, 0x80073b17); // HRESULT_FROM_WIN32(ERROR_MRM_NAMED_RESOURCE_NOT_FOUND)

            resourceCandidate = resourceMap.TryGetValue("xyz", resourceContext);
            Verify.IsNull(resourceCandidate);
        }

        [TestMethod]
        public void NoResourceFileWithContextTest()
        {
            var resourceManager = new ResourceManager("NoSuchFile.pri");
            var resourceMap = resourceManager.MainResourceMap;
            var resourceChildMap = resourceMap.GetSubtree("anyname");
            var resourceContext = resourceManager.CreateResourceContext();
            Verify.IsTrue(resourceContext.QualifierValues.ContainsKey(KnownResourceQualifierName.Language));

            // No resource file, and not handled by fallback 
            var ex = Verify.Throws<Exception>(() => resourceMap.GetValue("abc"));
            Verify.AreEqual((uint)ex.HResult, 0x80070490); // HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
            var resourceCandidate = resourceMap.TryGetValue("abc");
            Verify.IsNull(resourceCandidate);

            // add fallback handler
            resourceManager.ResourceNotFound += (sender, args) =>
            {
                if (args.Name == "abc")
                {
                    String value;
                    if (args.Context.QualifierValues.ContainsKey(KnownResourceQualifierName.Language))
                    {
                        if (args.Context.QualifierValues[KnownResourceQualifierName.Language].StartsWith("en-US"))
                        {
                            value = "USValue";
                        }
                        else if (args.Context.QualifierValues[KnownResourceQualifierName.Language].StartsWith("en-GB"))
                        {
                            value = "GBValue";
                        }
                        else
                        {
                            value = "OtherValue";
                        }
                    }
                    else
                    {
                        value = "UnknownValue";
                    }
                    var candidate = new ResourceCandidate(ResourceCandidateKind.String, value);
                    args.SetResolvedCandidate(candidate);
                }
            };

            resourceContext.QualifierValues[KnownResourceQualifierName.Language] = "en-US";
            resourceCandidate = resourceMap.GetValue("abc", resourceContext);
            Verify.AreEqual(resourceCandidate.Kind, ResourceCandidateKind.String);
            Verify.AreEqual(resourceCandidate.ValueAsString, "USValue");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "en-US");

            resourceContext.QualifierValues[KnownResourceQualifierName.Language] = "en-GB";
            resourceCandidate = resourceMap.GetValue("abc", resourceContext);
            Verify.AreEqual(resourceCandidate.Kind, ResourceCandidateKind.String);
            Verify.AreEqual(resourceCandidate.ValueAsString, "GBValue");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "en-GB");

            resourceContext.QualifierValues[KnownResourceQualifierName.Language] = "fr-FR";
            resourceCandidate = resourceMap.GetValue("abc", resourceContext);
            Verify.AreEqual(resourceCandidate.Kind, ResourceCandidateKind.String);
            Verify.AreEqual(resourceCandidate.ValueAsString, "OtherValue");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "fr-FR");

            resourceCandidate = resourceMap.TryGetValue("abc", resourceContext);
            Verify.AreEqual(resourceCandidate.Kind, ResourceCandidateKind.String);
            Verify.AreEqual(resourceCandidate.ValueAsString, "OtherValue");
            Verify.AreEqual(resourceCandidate.QualifierValues[KnownResourceQualifierName.Language], "fr-FR");

            ex = Verify.Throws<Exception>(() => resourceMap.GetValue("xyz", resourceContext));
            Verify.AreEqual((uint)ex.HResult, 0x80070490); // HRESULT_FROM_WIN32(ERROR_NOT_FOUND)

            resourceCandidate = resourceMap.TryGetValue("xyz", resourceContext);
            Verify.IsNull(resourceCandidate);
        }
    }
}
