using ShepherdConfigurator.Shell;

namespace ShepherdConfigurator.Tests;

public sealed class ExperimentalSettingCatalogTests
{
    [Fact]
    public void RecognizesExperimentalKeys()
    {
        Assert.True(ExperimentalSettingCatalog.IsExperimental("ReduceMenuMovieStutter"));
        Assert.True(ExperimentalSettingCatalog.IsExperimental("HardenLegacyThreadWrapper"));
        Assert.True(ExperimentalSettingCatalog.IsExperimental("EnableKeyboardPromptLabels"));
        Assert.False(ExperimentalSettingCatalog.IsExperimental("EnableFrameRateUnlock"));
    }

    [Fact]
    public void CountsExperimentalSettingsAcrossVisibleCategories()
    {
        int count = ExperimentalSettingCatalog.CountForCategories(["Display", "Movies", "Advanced"]);

        Assert.Equal(4, count);
    }

    [Fact]
    public void CountsBothExperimentalInputSettings()
    {
        Assert.Equal(2, ExperimentalSettingCatalog.CountForCategories(["Input"]));
    }
}
