import os
import re
import subprocess
import shutil

def fix_and_release(ts_folder):
    if not os.path.exists(ts_folder):
        print(f"Error: Folder '{ts_folder}' not found.")
        return

    for filename in os.listdir(ts_folder):
        if not filename.endswith(".ts"):
            continue
            
        ts_path = os.path.join(ts_folder, filename)
        shutil.copy2(ts_path, ts_path + ".bak")
        print(f"Processing: {filename}...")

        attempts = 0
        while attempts < 500:  # Safety limit
            attempts += 1
            result = subprocess.run(["lrelease", ts_path], capture_output=True, text=True)
            
            if result.returncode == 0:
                print(f"  -> Successfully generated {filename.replace('.ts', '.qm')}")
                break
            
            match = re.search(r"\.ts:(\d+):", result.stderr)
            if match:
                line_num = int(match.group(1))
                with open(ts_path, 'r', encoding='utf-8') as f:
                    content = f.read()

                lines = content.splitlines(keepends=True)
                pos_in_content = sum(len(l) for l in lines[:line_num-1])

                # 1. Identify if the parent <message> has numerus="yes"
                # We look backwards from the error position for the opening <message tag
                message_start = content.rfind('<message', 0, pos_in_content)
                message_end = content.find('</message>', pos_in_content)
                
                if message_start == -1 or message_end == -1:
                    print(f"  !! Could not bound message block near line {line_num}")
                    break

                message_block = content[message_start:message_end+10]
                is_numerus = 'numerus="yes"' in message_block

                # 2. Define the clean replacement
                if is_numerus:
                    # For plural forms, we need empty numerusform tags to stay valid
                    replacement = '<translation type="unfinished">\n            <numerusform></numerusform>\n            <numerusform></numerusform>\n        </translation>'
                else:
                    replacement = '<translation type="unfinished"></translation>'

                # 3. Locate the specific <translation> block within this message to swap it
                # Using DOTALL to handle the multi-line block you provided
                new_message_block = re.sub(r'<translation.*?>.*?</translation>', 
                                          replacement, 
                                          message_block, 
                                          flags=re.DOTALL)

                content = content[:message_start] + new_message_block + content[message_end+10:]

                with open(ts_path, 'w', encoding='utf-8') as f:
                    f.write(content)
                print(f"  -> Cleaned {'numerus' if is_numerus else 'standard'} block at line {line_num}")
            else:
                print(f"  -> Unrecoverable error in {filename}:\n{result.stderr}")
                break

if __name__ == "__main__":
    fix_and_release("ts")
