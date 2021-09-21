
# Helper function
compare_output_expected() {
    echo        "Unexpected output."
    echo -e     "### Output:\n$output"
    echo -e     "### Expected:\n$expected"
    [ "$output" = "$expected" ]
}

@test "devconf --template" {

    devconf --template _out/template

    expected=$(echo -e "factory_device.json\nuser_device.json")
    output="$(ls -1 _out/template)"
    compare_output_expected
}

@test "devconf --template [FILE(S) ALREADY EXIST]" {
    rm _out/template/factory_device.json

    run devconf --template _out/template

    [ "$status" != 0 ]
}

@test "devconf --input-dir ... --output-dir ..." {

    mkdir _out/in_dir
    # TODO cp "$PROJECT_DIR"/devices/SmartPlug.json _out/in_dir/factory_device.json
    devconf --template _out/in_dir
    mkdir _out/out_dir
    cp "$PROJECT_DIR"/devices/SmartLight.json _out/in_dir/factory_device.json

    run devconf -i _out/in_dir -o _out/out_dir

    echo "$output"
    [ "$status" = 0 ]
    [ -z "$output" ]
}
