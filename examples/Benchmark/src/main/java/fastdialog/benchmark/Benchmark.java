package fastdialog.benchmark;

import fastdialog.DialogFilter;
import fastdialog.FastDialog;
import org.openjdk.jmh.annotations.*;

import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.Throughput)
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@State(Scope.Benchmark)
@Warmup(iterations = 3, time = 1)
@Measurement(iterations = 5, time = 1)
@Fork(1)
public class Benchmark {

    private DialogFilter[] filters;
    private String[] descriptions;
    private String[] extensions;

    @Setup
    public void setup() {
        filters = new DialogFilter[] {
                new DialogFilter("Image Files (*.png;*.jpg;*.jpeg;*.webp)", "*.png;*.jpg;*.jpeg;*.webp"),
                new DialogFilter("Source Files (*.java;*.kt;*.cpp;*.h)", "*.java;*.kt;*.cpp;*.h"),
                new DialogFilter("Document Files (*.pdf;*.docx;*.txt)", "*.pdf;*.docx;*.txt"),
                new DialogFilter("All Files (*.*)", "*.*")
        };

        descriptions = new String[filters.length];
        extensions = new String[filters.length];
        for (int i = 0; i < filters.length; i++) {
            descriptions[i] = filters[i].getDescription();
            extensions[i] = filters[i].getExtensionPattern();
        }
    }

    @org.openjdk.jmh.annotations.Benchmark
    public boolean benchmarkNativeAvailabilityCheck() {
        return FastDialog.isNativeAvailable();
    }

    @org.openjdk.jmh.annotations.Benchmark
    public DialogFilter[] benchmarkFilterInstantiation() {
        return new DialogFilter[] {
                new DialogFilter("Image Files (*.png;*.jpg)", "*.png;*.jpg"),
                new DialogFilter("All Files (*.*)", "*.*")
        };
    }

    @org.openjdk.jmh.annotations.Benchmark
    public int benchmarkFilterArrayIteration() {
        int lengthSum = 0;
        for (DialogFilter f : filters) {
            lengthSum += f.getDescription().length() + f.getExtensionPattern().length();
        }
        return lengthSum;
    }
}
