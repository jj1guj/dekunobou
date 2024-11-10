use cmake;

fn main() {
    let dst = cmake::build("src/cpp");
    println!("cargo:rustc-link-search=native={}", dst.display());

    println!("cargo:rustc-link-lib=static=dekunobou");
    println!("cargo:rustc-link-lib=gomp");

    // C++ソースコードの場合は必ずこれを追加すること
    println!("cargo:rustc-link-lib=dylib=stdc++");
}
