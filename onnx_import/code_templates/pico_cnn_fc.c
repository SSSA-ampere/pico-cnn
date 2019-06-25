{% if bias_buffer %}
fully_connected_naive({{input_buffer.start_ptr}}, {{input_size}},
                      {{output_buffer.start_ptr}}, {{output_size}},
                      {{weight_buffer.start_ptr}}, {{bias_buffer.start_ptr}});
{% else %}
fully_connected_naive({{input_buffer.start_ptr}}, {{input_size}},
                      {{output_buffer.start_ptr}}, {{output_size}},
                      {{weight_buffer.start_ptr}}, NULL);
{% endif %}
