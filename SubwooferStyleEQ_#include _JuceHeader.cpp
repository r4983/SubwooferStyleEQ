#include <JuceHeader.h>

class SubwooferStyleEQ
{
public:
    SubwooferStyleEQ() {}

    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = 2;

        lowShelfFilter.state = dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 50.0f, 0.7f, Decibels::decibelsToGain(6.0f));
        lowMidFilter.state = dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 120.0f, 1.0f, Decibels::decibelsToGain(3.0f));
        highMidCutFilter.state = dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 300.0f, 1.2f, Decibels::decibelsToGain(-4.0f));
        highCutFilter.state = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 10000.0f, 0.7f);

        processorChain.prepare(spec);
    }

    void processBlock(AudioBuffer<float>& buffer)
    {
        dsp::AudioBlock<float> audioBlock(buffer);
        processorChain.process(dsp::ProcessContextReplacing<float>(audioBlock));
    }

private:
    dsp::ProcessorChain<dsp::IIR::Filter<float>, dsp::IIR::Filter<float>, dsp::IIR::Filter<float>, dsp::IIR::Filter<float>> processorChain;

    dsp::IIR::Filter<float>& lowShelfFilter = processorChain.get<0>();
    dsp::IIR::Filter<float>& lowMidFilter = processorChain.get<1>();
    dsp::IIR::Filter<float>& highMidCutFilter = processorChain.get<2>();
    dsp::IIR::Filter<float>& highCutFilter = processorChain.get<3>();
};
